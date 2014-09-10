#include "umls.h"

umls::umls()
{

}


umls::~umls() {}


void umls::parseUMLS(char* filename, char* MRSTYFile, char* vforbiddenFile, char* eforbiddenFile, char* VertexSemanticLabelFile, char* VertexFile, char* EdgeinformationFile, char* EdgeFile, char* GraphFile)
{
	ifstream infile(filename);
	ifstream inMRSTY(MRSTYFile);
	ifstream in_v_forbidden(vforbiddenFile);
	ifstream in_e_forbidden(eforbiddenFile);
	
	ofstream out_vertexsem(VertexSemanticLabelFile,ios_base::out);
	ofstream out_vertex(VertexFile,ios_base::out);
	ofstream out_edgeinfor(EdgeinformationFile,ios_base::out);
	ofstream out_edge(EdgeFile,ios_base::out);
	ofstream out_graph(GraphFile,ios_base::out);

	if (!infile || !inMRSTY || !in_e_forbidden || !in_v_forbidden) {
		cout << "Error: At least one input file cannot be opened." << endl;
		exit(0);
	}
	if(!out_vertexsem || !out_vertex || !out_edgeinfor || !out_edge || !out_graph)
	{
		cout<<"Error: Failed to create at least one output file"<<endl;
		exit(0);
	}
	

	//read MRREL.RRF file
	set<string> vertex;
	set<string> edgeinfor;
	string buf;
	string sub;
	string left_v;
	string right_v;

	int n;
	while(getline(infile, buf))
	{
		n=0;
		while (buf.find("|") != string::npos) 
		{
			sub = buf.substr(0, buf.find("|"));
			buf.erase(0, buf.find("|")+1);
			n++;
			if(n==1)
			{
				left_v=sub;
			}
			if(n==5)
			{
				right_v=sub;
				if(left_v.compare(right_v)==0)
					break;
			}
			if(n==8)
			{
				
				if(sub!="")
				{
					edgeinfor.insert(sub);
					vertex.insert(left_v);
					vertex.insert(right_v);
					break;
				}
				else
					break;
			}
		}
	}
	infile.close();
	
	//Read MRSTY.RRF file to generate vertex semantic label file
	set<string> VSemantics;
	while(getline(inMRSTY,buf))
	{
		for(int i=0;i<3;i++)
			buf.erase(0, buf.find("|")+1);

		VSemantics.insert(buf.substr(0, buf.find("|")));
	}
	
	for(set<string>::iterator it=VSemantics.begin(); it!=VSemantics.end(); it++)
	{
		out_vertexsem<<*it<<endl;
	}	
	cout<<VertexSemanticLabelFile<<" successfully generated!"<<endl;	
	inMRSTY.close();


	//Read v forbidden File to check the validity of inputs
	set<string> VSemForbid;
	while(getline(in_v_forbidden, buf))
	{
		if(VSemantics.find(buf)==VSemantics.end()){
			cout<<"Error in v forbidden File. "<<endl;
			cout<<buf<<" cannot be found in VSemantics"<<endl;
			exit(0);
		}else
			VSemForbid.insert(buf);
	}
	in_v_forbidden.close();


	//construct vertex-int and edge information - int map
	map<string, int> vertex_int;
	map<string, int> edgeinfor_int;
	
	n=0;
	set<string>::iterator it;
	for(it=vertex.begin();it!=vertex.end();it++)
	{
		out_vertex << *it <<endl;
		vertex_int.insert(pair<string,int>(*it,n));
		n++;
	}

	n=0;
	for(it=edgeinfor.begin();it!=edgeinfor.end();it++)
	{
		out_edgeinfor << *it <<endl;
		edgeinfor_int.insert(pair<string,int>(*it,n));
		n++;
	}
	
	out_vertex.close();
	out_edgeinfor.close();

	

	//Read MRSTY.RRF file again to generate forbidden vertex list
	set<int> VertexForbiddenList;
	inMRSTY.open(MRSTYFile);
	while(getline(inMRSTY,buf))
	{
		string CUI_tmp=buf.substr(0, buf.find("|"));
		for(int i=0;i<3;i++)
			buf.erase(0, buf.find("|")+1);
			
		string sem_tmp=buf.substr(0, buf.find("|"));
		
		if(VSemForbid.find(sem_tmp)!=VSemForbid.end())
		{
			map<string, int>::iterator iter_cui2int=vertex_int.find(CUI_tmp);
			if(iter_cui2int!=vertex_int.end())
				VertexForbiddenList.insert(iter_cui2int->second);
			else
				cout<<CUI_tmp<<" is not found in the semantic network."<<endl;
		}

	}
	cout<<"vertex forbiddent list size: "<<VertexForbiddenList.size()<<endl;
	inMRSTY.close();
	
	
	//output edge file
	infile.open(filename);
	
	n=0;
	map<string,int>::iterator left_it;
	map<string,int>::iterator right_it;
	map<string,int>::iterator edgeinfo_it;
	while(getline(infile, buf))
	{
		while (buf.find("|") != string::npos) 
		{
			sub = buf.substr(0, buf.find("|"));
			buf.erase(0, buf.find("|")+1);
			n++;
			if(n==1)
			{
				left_v=sub;
				left_it=vertex_int.find(sub);
				if(!(left_it!=vertex_int.end()))
				{
					break;
				}
			}
			if(n==5)
			{
				right_v=sub;
				right_it=vertex_int.find(sub);
				if(left_v.compare(right_v)==0)
					break;
					
				if(!(right_it!=vertex_int.end()))
				{
					break;
				}
			}
			if(n==8)
			{
				if(sub!="")
				{
					edgeinfo_it=edgeinfor_int.find(sub);
					if(edgeinfo_it!=edgeinfor_int.end())
					{
						out_edge << left_it->second <<" "<<right_it->second<<" "<<edgeinfo_it->second<<endl;
						break;
					}
				}
				break;
			}
		}
		n=0;
	}
	out_edge.close();
	infile.close();

	//read forbidden edge file
	vector<string> forbiddenEdgeList;
	set<int> forbiddenEdgeIDList;
	while(getline(in_e_forbidden, buf))
		forbiddenEdgeList.push_back(buf);	
	
	for(vector<string>::iterator iter_forbid=forbiddenEdgeList.begin(); iter_forbid!=forbiddenEdgeList.end(); iter_forbid++)
	{
		map<string,int>::iterator iter_edgeinf=edgeinfor_int.find(*iter_forbid);
		if(iter_edgeinf!=edgeinfor_int.end())
			forbiddenEdgeIDList.insert(iter_edgeinf->second);
		else{
			cout<<"Error in reading forbidden edge file."<<endl;
			exit(0);			
		}			
	}
	in_e_forbidden.close();
	
	//outout format graph file

	vector<set<int> > graph_info = vector<set<int> >(vertex.size(),set<int>());//The adjacent vertices are always ordered. This is important for further processing.
	
	int nnn=0;
	int vvv=0;
	int fff=0;
	infile.open(EdgeFile);
	while(getline(infile, buf))
	{
			int left=-1;
			int right=-1;
			int edge=-1;	
			nnn++;	
			sub = buf.substr(0, buf.find(" "));
			buf.erase(0, buf.find(" ")+1);
			istringstream(sub) >> left;

			sub = buf.substr(0, buf.find(" "));
			buf.erase(0, buf.find(" ")+1);
			istringstream(sub) >> right;
			
			if(left==right)
			{
				cout<<"Self-loop Detected at vertex no: "<<left<<endl;
				continue;//no self-loop will be built into the graph
			}
			else
				vvv++;

			istringstream(buf) >> edge;

			if(forbiddenEdgeIDList.find(edge)==forbiddenEdgeIDList.end() && VertexForbiddenList.find(left)==VertexForbiddenList.end() && VertexForbiddenList.find(right)==VertexForbiddenList.end())
			{
				graph_info[right].insert(left);
				fff++;
				
			}
	}
	infile.close();
	
	cout<<"Number of lines processed: "<<nnn<<endl;
	cout<<"Number of valid lines (excluding self-loop edges): "<<vvv<<endl;
	cout<<"Number of effective lines in graph edge construction (excluding forbidden edge and vertex labels): "<<fff<<endl;
	out_graph << "graph_for_greach" <<endl;
	out_graph << vertex.size() <<endl;
	int num_edge=0;

	
	set<int>::iterator giter;
	for(int i=0; i!=vertex.size();i++)
	{
		out_graph << i <<": "; 
		for(giter=graph_info[i].begin();giter!=graph_info[i].end();giter++)
		{
			out_graph << *giter <<" ";
			
			num_edge++;
		}
		out_graph << "#"<<endl;
		
	}
	out_graph.close();
	
	cout<<"Number of edges in the generated graph:"<<num_edge<<", number of vertices in the generated graph:"<<vertex.size()<<endl;
	cout<<"Graph generation process (excluding forbidden vertex and edge list) is successful!"<<endl;
}


