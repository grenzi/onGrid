
#include "Greedylabel.h"
#include <sys/time.h>

Greedylabel::Greedylabel(Graph& graph):g(graph){

}

Greedylabel::~Greedylabel() {
}

void Greedylabel::readLabelSize(vector<vector<Label> >& VertexLabelList, char* FileName){

	int lsize;
	string buf;	

	ifstream ifstr_(FileName);
	if(!ifstr_)
	{
		cout<<"error in locating File!"<<endl;
	}
	int i=0;
	while(getline(ifstr_, buf))
	{
		istringstream(buf) >> lsize;
		VertexLabelList[i]=vector<Label>(lsize);
		i++;
	}
	ifstr_.close();
}

void Greedylabel::readLabelSize(vector<vector<LabelDiff> >& VertexLabelList, char* FileName){

	int lsize;
	string buf;	

	ifstream ifstr_(FileName);
	if(!ifstr_)
	{
		cout<<"error in locating File!"<<endl;
	}
	int i=0;
	while(getline(ifstr_, buf))
	{
		istringstream(buf) >> lsize;
		VertexLabelList[i]=vector<LabelDiff>(lsize);
		i++;
	}
	ifstr_.close();
}

void Greedylabel::run(int length_limit, char* LinFile, char* LoutFile, char* LinoutsameFile, char* LinoutdifferentFile)
{
	int number_of_vertices;
	number_of_vertices=g.num_vertices();
	l_in = vector<vector<Label> >(g.num_vertices());
	l_out = vector<vector<Label> >(g.num_vertices());
	l_in_out_same = vector<vector<Label> >(g.num_vertices());
	l_in_out_diff = vector<vector<LabelDiff> >(g.num_vertices());
	
	int i;
	
	vector<int> l_in_labelsize=vector<int>(g.num_vertices(),0);
	vector<int> l_out_labelsize=vector<int>(g.num_vertices(),0);
	vector<int> l_in_out_same_labelsize=vector<int>(g.num_vertices(),0);
	vector<int> l_in_out_diff_labelsize=vector<int>(g.num_vertices(),0);

	readLabelSize(l_in, LinFile);
	readLabelSize(l_out, LoutFile);
	readLabelSize(l_in_out_same, LinoutsameFile);
	readLabelSize(l_in_out_diff, LinoutdifferentFile);
	

	//handling sink and source vertices
	vector<int> step1;
	int num_v_outzero=0;
	int num_v_inzero=0;
	labelEstPara label_est=labelEstPara();
	for(i=0;i<g.num_vertices();i++)
	{
		if((g.graph[i].outList.size()==0)&&(g.graph[i].inList.size()>=10))
		{
			num_v_outzero++;
			step1.push_back(i);
		}
		if((g.graph[i].inList.size()==0)&&(g.graph[i].outList.size()>=10))
		{
			num_v_inzero++;
			step1.push_back(i);
		}
	}
	cout<<"Number of sinks:"<<num_v_outzero<<endl;
	cout<<"Number of sources:"<<num_v_inzero<<endl;
	cout<<"Total number of sinks and sources:"<<step1.size()<<endl;
	
	for(i=0;i<step1.size();i++)//code simplified on 7/29/2011 by YX
	{
		cout<<i<<endl;
		vector<int>::iterator erase_iterstep1;//erase the edge go to the center point
		for(erase_iterstep1=g.graph[step1[i]].outList.begin();erase_iterstep1!=g.graph[step1[i]].outList.end();erase_iterstep1++)
		{
			vector<int>::iterator deletepointer=find(g.graph[*erase_iterstep1].inList.begin(), g.graph[*erase_iterstep1].inList.end(), step1[i]);
			if (deletepointer==g.graph[*erase_iterstep1].inList.end())
				cout<<"Error in clearing source vertices"<<endl;
			else
				g.graph[*erase_iterstep1].inList.erase(deletepointer);
		}
		g.graph[step1[i]].outList.clear();
		
		for(erase_iterstep1=g.graph[step1[i]].inList.begin();erase_iterstep1!=g.graph[step1[i]].inList.end();erase_iterstep1++)
		{

			vector<int>::iterator deletepointer=find(g.graph[*erase_iterstep1].outList.begin(), g.graph[*erase_iterstep1].outList.end(), step1[i]);
			if (deletepointer==g.graph[*erase_iterstep1].outList.end())
				cout<<"Error in clearing sink vertices"<<endl;
			else
				g.graph[*erase_iterstep1].outList.erase(deletepointer);			
		}
		g.graph[step1[i]].inList.clear();
	}
	step1.clear();
	//handling sink and source vertices done

	
	
	int cut_degree=0;
//	char* label="./cutdegree_0";
//	ofstream out(label, ios_base::out|ios_base::app);
//	out <<"*******************************"<<endl;
//	out <<"cut_degree:"<<cut_degree<<endl;
//	out << "num_v_outzero:"<<num_v_outzero<<endl;
//	out << "num_v_inzero:"<<num_v_inzero<<endl;


	set<int> delete_nodes;
	set<intpair,intpaircomp> v_degree;
	intpair degree;
	
	vector<int> vd=vector<int>(g.num_vertices());
	
	for(i=0;i<g.num_vertices();i++)
	{
		degree.first=i;
		degree.second=0;
		degree.second=degree.second+g.graph[i].inList.size();
		degree.second=degree.second+g.graph[i].outList.size();
		vd[i]=degree.second;
		v_degree.insert(degree);
	}
	
	set<intpair,intpaircomp>::iterator v_degreeit;
	intpair findit;
	set<intpair,intpaircomp>::iterator updateit;
	
	struct timeval after_time, before_time;

	gettimeofday(&before_time,NULL);

	int num_vertices_del;
	for(i=0;i<g.num_vertices();i++)
	{
		
		if(i%1000==0)
		{
			cout<<"Top "<<i<<" vertices done."<<endl;
			cout<<"Current label size (Bytes):"<<(label_est.inout_same+label_est.only_in+label_est.only_out)*6+label_est.inout_different*8<<endl;				
		}
		v_degreeit=v_degree.begin();	
		if(v_degreeit->second<=cut_degree)
		{
			gettimeofday(&after_time,NULL);
			double duration=(after_time.tv_sec - before_time.tv_sec)*1000.0 + (after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
			
			cout<<"Labeling time:"<<duration<<endl;
			cout<<"Estimated total label size (Bytes): "<<(label_est.inout_same+label_est.only_in+label_est.only_out)*6+label_est.inout_different*8<<endl;
			cout<<"Estimated total label size by in and out only (Bytes): "<<(label_est.inout_same*2+label_est.only_in+label_est.only_out+label_est.inout_different*2)*6<<endl;
			cout<<"Estimated total label size by in and out only which include self-vertex (Bytes): "<<(label_est.inout_same*2+label_est.only_in+label_est.only_out+label_est.inout_different*2+g.num_vertices()*2)*6<<endl;
			cout<<"Labels with same in and out: "<<label_est.inout_same<<endl;
			cout<<"Label with only in: "<<label_est.only_in<<endl;
			cout<<"Label with only out: "<<label_est.only_out<<endl;
			cout<<"Label with different in and out: "<<label_est.inout_different<<endl;
			cout<<"Total number of vertices processed:"<<i<<endl;			

			break;
		}
		//delete_nodes.insert(v_degreeit->first);
		//cout<<"No. "<<i<<" processing"<<endl;
		BFSLabel(g, v_degreeit->first, length_limit, label_est, l_in_labelsize,l_out_labelsize,l_in_out_same_labelsize,l_in_out_diff_labelsize, l_in, l_out, l_in_out_same, l_in_out_diff, l_special);//label for a center point with high degree
		//cout<<"No. "<<i<<" processed"<<endl;
		
		vector<int>::iterator erase_iter;//erase the edge go to the center point
		for(erase_iter=g.graph[v_degreeit->first].outList.begin();erase_iter!=g.graph[v_degreeit->first].outList.end();erase_iter++)
		{

			vector<int>::iterator erase_it=find(g.graph[*erase_iter].inList.begin(), g.graph[*erase_iter].inList.end(), v_degreeit->first);
			if (erase_it==g.graph[*erase_iter].inList.end())
				cout<<"Error in decentralizing"<<endl;
			else
			{
				g.graph[*erase_iter].inList.erase(erase_it);

				findit.first=*erase_iter;
				findit.second=vd[*erase_iter];
				updateit=v_degree.find(findit);
				degree.first=updateit->first;
				//if(updateit->second<65535)//This is to ensure vertices with original degree 65535 or larger will be processed first so their degrees will not be updated.
				//{
					degree.second=updateit->second-1;
					vd[*erase_iter]=updateit->second-1;
					v_degree.erase(updateit);
					v_degree.insert(degree);
				//}					
			}
		}
		g.graph[v_degreeit->first].outList.clear();
		
		for(erase_iter=g.graph[v_degreeit->first].inList.begin();erase_iter!=g.graph[v_degreeit->first].inList.end();erase_iter++)
		{

			vector<int>::iterator erase_it=find(g.graph[*erase_iter].outList.begin(), g.graph[*erase_iter].outList.end(), v_degreeit->first);
			if (erase_it==g.graph[*erase_iter].outList.end())
				cout<<"Error in decentralizing"<<endl;
			else
			{
				g.graph[*erase_iter].outList.erase(erase_it);

				findit.first=*erase_iter;
				findit.second=vd[*erase_iter];
				updateit=v_degree.find(findit);
				degree.first=updateit->first;
				//if(updateit->second<65535)//This is to ensure vertices with original degree 65535 or larger will be processed first so their degrees will not be updated.
				//{
					degree.second=updateit->second-1;
					vd[*erase_iter]=updateit->second-1;
					v_degree.erase(updateit);
					v_degree.insert(degree);
				//}					
			}
		}
		g.graph[v_degreeit->first].inList.clear();

		v_degree.erase(v_degree.begin());
		
	}
	

	
	long int verify=0;
	for(i=0;i<g.num_vertices();i++)
	{
		verify=verify+l_in[i].size()*6;
		verify=verify+l_out[i].size()*6;
		verify=verify+l_in_out_same[i].size()*6;
		verify=verify+l_in_out_diff[i].size()*8;
	}
	cout<<"Total label size verification: "<<verify<<endl;
	cout<<"Special label records (each contains 6 integers): "<<l_special.size()<<endl;
}
