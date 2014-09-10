#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include "Greedylabel.h"
#include "BFS.h"
#include <sys/time.h>
#include <algorithm>
using namespace std;

#define MAX 100000


struct Labelcomp
{
	bool operator() (const Label& l1, const Label&l2) const{
		int pow16;
		int pow8;
		pow16=int(pow(2.0,16.0));
		pow8=int(pow(2.0,8.0));
		int index1;
		int index2;
		index1=l1.dest1*pow16+l1.dest2*pow8+l1.dest3;
		index2=l2.dest1*pow16+l2.dest2*pow8+l2.dest3;
		if(index1<index2)
			return true;
		else
			return false;
	}
} mylabelcomp;

typedef pair<int,int> vertexPair;

void pathdecoder(ofstream& out_result, vector<int>& path, vector<string>& VERTEX, map<string,string>& vertex_label, multimap<vertexPair,int>& edgeList, vector<string>& edgeLabelList, set<int>& forbiddenEdgeIDList)
{

	out_result<<"Path Length: "<<path.size()-1<<"	";
	string v_CUI=VERTEX[path[0]];
	map<string,string>::iterator iter_vLabel=vertex_label.find(v_CUI);
	if(iter_vLabel==vertex_label.end()){
		cout<<v_CUI<<" does not exists in MSCONSO.RRF. Unexpected Error."<<endl;
		exit(0);
	}else
		out_result<<v_CUI<<":"<<iter_vLabel->second;
		
	for(int i=0; i<path.size()-1; i++)
	{
		vertexPair e_tmp;
		int left=path[i];
		int right=path[i+1];
		e_tmp.first=left;
		e_tmp.second=right;
		pair<multimap<vertexPair,int>::iterator, multimap<vertexPair,int>::iterator> ret=edgeList.equal_range(e_tmp);
		multimap<vertexPair,int>::iterator edgeit=edgeList.find(e_tmp);
		if(distance(ret.first, ret.second)>0){
			multimap<vertexPair,int>::iterator edgeit;
			for (edgeit=ret.first; edgeit!=ret.second; edgeit++)
			{
				if(forbiddenEdgeIDList.find(edgeit->second)==forbiddenEdgeIDList.end()){
					string edge_label_tmp=edgeLabelList[edgeit->second];
					out_result<<"	"<<edge_label_tmp;
					break;
				}
			}
			if(edgeit==ret.second)
				cout<<"Error in kDLS semantic restriction processing!"<<endl;
		}else{
			cout<<"Unexpected Error: An edge does not exist in the edge ID file!"<<endl;
			cout<<left<<","<<right<<endl;
			exit(0);
		}
		v_CUI=VERTEX[right];
		iter_vLabel=vertex_label.find(v_CUI);
		if(iter_vLabel==vertex_label.end())
		{
			cout<<v_CUI<<" does not exists in MSCONSO.RRF. Unexpected Error."<<endl;
			exit(0);
		}else
			out_result<<"	"<<v_CUI<<":"<<iter_vLabel->second;
	}
	out_result<<endl;
}

int main(int argc, char* argv[])
{
	struct timeval after_time, before_time;
	float query_time,construct_time;

	char* filename=argv[1];
	char* vertexCUIfile=argv[2];
	char* vertexLabelfile=argv[3];
	char* edgeIDFile=argv[4];
	char* edgeLabelFile=argv[5];
	int length_limit=atoi(argv[6]);
	char* sinksourcefile=argv[7];
	char* in_label=argv[8];
	char* out_label=argv[9];
	char* in_out_same_label=argv[10];
	char* in_out_diff_label=argv[11];
	char* special_label=argv[12];
	char* eforbiddenFile=argv[13];
	char* queryfile=argv[14];
	char* resultFile=argv[15];
	char* rankFile = argv[16];
	double EXPBASE=atof(argv[17]);
	
	ifstream infile(filename);
	ifstream in_vCUI(vertexCUIfile);
	ifstream in_vLabel(vertexLabelfile);
	ifstream in_edgeID(edgeIDFile);
	ifstream in_edgeLabel(edgeLabelFile);	
	ifstream in_sinksource(sinksourcefile);
	ifstream in_query(queryfile);
	ifstream in_e_forbidden(eforbiddenFile);
	ifstream in_rankfile(rankFile);

	if(!infile || !in_vCUI || !in_vLabel || !in_edgeID || !in_edgeLabel ||!in_sinksource ||!in_query ||!in_e_forbidden || !in_rankfile)
	{
		cout<<"Error in opening at least one input file. Exit."<<endl;
		exit(0);
	}
	Graph g(infile, in_rankfile);
	cout<<"Number of vertices: "<<g.num_vertices()<<endl;
	infile.close();
	in_rankfile.close();
	Greedylabel kDLS=Greedylabel(g, in_label, out_label, in_out_same_label, in_out_diff_label, special_label);
	
	gettimeofday(&before_time,NULL);

	cout<<"***********************"<<endl;
		
	//read the vertex CUI file into a vector
	string buf;
	vector<string> VERTEX; 
	int n=0;
	while (getline(in_vCUI, buf)) {
		VERTEX.push_back(buf);
		n++;
	}
	in_vCUI.close();
	//******************************************************************
	
	//read the vertex label file, each vertex map to a meanning
	map<string,string> vertex_label;              //map a vertex to a meaning
	while (getline(in_vLabel, buf)) {
		string sub=buf.substr(0, buf.find("	"));
		buf.erase(0, buf.find("	")+1);
		vertex_label.insert(pair<string,string>(sub,buf));
	}
	in_vLabel.close();
	//***************************************************************
	
	//read the edge ID file
	multimap<vertexPair,int> edgeList;
	n=0;
	while (getline(in_edgeID, buf)) {//first-->second
		vertexPair e;
		istringstream(buf.substr(0, buf.find(" "))) >> e.second;
		buf.erase(0, buf.find(" ")+1);
		istringstream(buf.substr(0, buf.find(" "))) >> e.first;
		buf.erase(0, buf.find(" ")+1);
		int edgeLabelID;
		istringstream(buf) >> edgeLabelID;
		edgeList.insert(pair<vertexPair,int>(e,edgeLabelID));
		n++;
	}
	in_edgeID.close();
	cout<<"Total number of edges loaded from edgeIDFile: "<<n<<endl;
	//**************************************************
	
	//read the edge label file
	vector<string> edgeLabelList;
	n=0;
	while (getline(in_edgeLabel, buf)) {
		edgeLabelList.push_back(buf);
		n++;
	}
	in_edgeLabel.close();
	cout<<"Total number of edge labels loaded from edgeLabelFile: "<<n<<endl;
	//*********************************************

	//read forbidden edge file
	vector<string> forbiddenEdgeList;
	set<int> forbiddenEdgeIDList;
	while(getline(in_e_forbidden, buf))
		forbiddenEdgeList.push_back(buf);	
	
	for(vector<string>::iterator iter_forbid=forbiddenEdgeList.begin(); iter_forbid!=forbiddenEdgeList.end(); iter_forbid++)
	{
		vector<string>::iterator forbid_edge_index=find(edgeLabelList.begin(),edgeLabelList.end(),*iter_forbid);
		if(forbid_edge_index!=edgeLabelList.end())
			forbiddenEdgeIDList.insert(distance(edgeLabelList.begin(),forbid_edge_index));
		else{
			cout<<"Error in reading forbidden edge file."<<endl;
			exit(0);		
		}			
	}
	in_e_forbidden.close();	
	
	//read the sinksource file
	set<int> sinksourcenodes;
	while (getline(in_sinksource, buf)) {
		int i;
		istringstream(buf) >> i;
		sinksourcenodes.insert(i);
	}
	in_sinksource.close();
	cout<<"Number of sinks and sources:"<<sinksourcenodes.size()<<endl;	
	//*********************************************

	//read the query file, each line contains two pair of CUIs separated by tab.
	multimap<string,string> queryPairs;              //map a vertex to a meaning
	while (getline(in_query, buf)) {
		string sub=buf.substr(0, buf.find("	"));
		buf.erase(0, buf.find("	")+1);
		queryPairs.insert(pair<string,string>(sub,buf));
	}
	cout<<"Number of query Pairs: "<<queryPairs.size()<<endl;
	in_query.close();
	//***************************************************************	
	
	ofstream out_result(resultFile);
	
	if (!out_result) {
		cout<<"Error in opening output file. Exit."<<endl;
		exit(0);
	}
	
	for(multimap<string,string>::iterator iter_query=queryPairs.begin(); iter_query!=queryPairs.end(); iter_query++)
	{
		string queryCUI_source=iter_query->first;
		string queryCUI_destination=iter_query->second;
		
		int ss,tt;
		out_result<<"Starting Query: Source CUI: "<<queryCUI_source<<" Destination CUI: "<<queryCUI_destination<<endl;
		//************************************************************************************************
		vector<string>::iterator vint_it=find(VERTEX.begin(), VERTEX.end(), queryCUI_source);
		if(vint_it!=VERTEX.end())
		{
			ss=distance(VERTEX.begin(), vint_it);
		}
		else
		{
			out_result<<queryCUI_source<<" is not connected to the semantic network by any edge. So no path can be returned."<<endl;
			out_result<<"End Query: Source CUI: "<<queryCUI_source<<" Destination CUI: "<<queryCUI_destination<<endl<<endl<<endl;
			continue;
		}
		
		vint_it=find(VERTEX.begin(), VERTEX.end(), queryCUI_destination);
		if(vint_it!=VERTEX.end())
		{
			tt=distance(VERTEX.begin(), vint_it);
		}
		else
		{
			out_result<<queryCUI_destination<<" is not connected to the semantic network by any edge. So no path can be returned."<<endl;
			out_result<<"End Query: Source CUI: "<<queryCUI_source<<" Destination CUI: "<<queryCUI_destination<<endl<<endl<<endl;
			continue;
		}
		
		out_result<<"Source CUI ID: "<<ss<<"; Destination CUI ID: "<<tt<<endl;
		
		vector<int> path;
		kDLS.find_oneshortestpath(ss,tt, path);
		if(path.size()!=0)
		{
			out_result<<"A shortest path from source to destination:"<<endl;
			pathdecoder(out_result, path, VERTEX, vertex_label, edgeList, edgeLabelList, forbiddenEdgeIDList);
		}
		
		path.clear();
		kDLS.find_oneshortestpath(tt,ss, path);
		if(path.size()!=0)
		{
			out_result<<"A shortest path from destination to source:"<<endl;
			pathdecoder(out_result, path, VERTEX, vertex_label, edgeList, edgeLabelList, forbiddenEdgeIDList);
		}
		
		out_result<<"***************************************************************************************"<<endl;
		
		vector<vector<int> > allshortestpath;
		kDLS.find_allshortestpath(ss,tt,allshortestpath,EXPBASE);
		if(allshortestpath.size()!=0)
		{
			out_result<<allshortestpath.size()<<" shortest simple kDLS paths from source to destination:"<<endl;
			for(vector<vector<int> >::iterator iter_allshortestpath=allshortestpath.begin(); iter_allshortestpath!=allshortestpath.end(); iter_allshortestpath++)
				pathdecoder(out_result, *iter_allshortestpath, VERTEX, vertex_label, edgeList, edgeLabelList, forbiddenEdgeIDList);
		}
		
		allshortestpath.clear();
		kDLS.find_allshortestpath(tt,ss,allshortestpath,EXPBASE);
		if(allshortestpath.size()!=0)
		{
			out_result<<allshortestpath.size()<<" shortest simple kDLS paths from destination to source:"<<endl;
			for(vector<vector<int> >::iterator iter_allshortestpath=allshortestpath.begin(); iter_allshortestpath!=allshortestpath.end(); iter_allshortestpath++)
				pathdecoder(out_result, *iter_allshortestpath, VERTEX, vertex_label, edgeList, edgeLabelList, forbiddenEdgeIDList);
		}		
		out_result<<"************************************************************************************"<<endl;
		
		
		vector<vector<int> > allpath;
		kDLS.find_allpath(ss,tt,allpath,EXPBASE);
		if(allpath.size()!=0)
		{
			out_result<<allpath.size()<<" simple kDLS paths from source to destination:"<<endl;
			for(vector<vector<int> >::iterator iter_allpath=allpath.begin(); iter_allpath!=allpath.end(); iter_allpath++)
				pathdecoder(out_result, *iter_allpath, VERTEX, vertex_label, edgeList, edgeLabelList, forbiddenEdgeIDList);
		}
		
		allpath.clear();
		kDLS.find_allpath(tt,ss,allpath,EXPBASE);
		if(allpath.size()!=0)
		{
			out_result<<allpath.size()<<" simple kDLS paths from destination to source:"<<endl;
			for(vector<vector<int> >::iterator iter_allpath=allpath.begin(); iter_allpath!=allpath.end(); iter_allpath++)
				pathdecoder(out_result, *iter_allpath, VERTEX, vertex_label, edgeList, edgeLabelList, forbiddenEdgeIDList);
		}
		
		out_result<<"End Query: Source CUI: "<<queryCUI_source<<" Destination CUI: "<<queryCUI_destination<<endl<<endl<<endl;
		//************************************************************************************************
	}
	gettimeofday(&after_time,NULL);
	construct_time=(after_time.tv_sec - before_time.tv_sec)*1000.0 + (after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
	cout<<"Total construction time:"<<construct_time<<endl;	
}
