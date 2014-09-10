
#include "Greedylabel.h"
#include <sys/time.h>

Greedylabel::Greedylabel(Graph& graph):g(graph){
	//threshold=0.1f;
}

//Greedylabel::Greedylabel(Graph& graph):g(graph)
//{
//
//}

Greedylabel::~Greedylabel() {
}

void Greedylabel::run(int length_limit, char* LinFile, char* LoutFile, char* LinoutsameFile, char* LinoutdifferentFile, char* sinksourcefile)
{
	
	int number_of_vertices;
	number_of_vertices=g.num_vertices();
	l_in = vector<vector<Label> >(g.num_vertices());
	l_out = vector<vector<Label> >(g.num_vertices());
	
	vector<int> l_in_labelsize=vector<int>(g.num_vertices(),0);
	vector<int> l_out_labelsize=vector<int>(g.num_vertices(),0);
	vector<int> l_in_out_same_labelsize=vector<int>(g.num_vertices(),0);
	vector<int> l_in_out_different_labelsize=vector<int>(g.num_vertices(),0);
	
	int i;
	for(i=0;i<g.num_vertices();i++)
	{
		l_in_labelsize[i]=0;
		l_out_labelsize[i]=0;
	}
	
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

	ofstream out_sinksource_nodes(sinksourcefile,ios_base::out);
	for(i=0;i<step1.size();i++)//code simplified on 7/29/2011 by YX
	{
		cout<<i<<endl;
		out_sinksource_nodes<<step1[i]<<endl;
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
	out_sinksource_nodes.close();		
	
	int edge_num=0;
	for(i=0;i<g.num_vertices();i++)
	{
		edge_num=edge_num+g.graph[i].outList.size();
	}
	cout<<"Total number of edges after processing sinks and sources:"<<edge_num<<endl;
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
				/*gettimeofday(&after_time,NULL);		
				duration=(after_time.tv_sec - before_time.tv_sec)*1000.0 + (after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
				cout<<"time:"<<duration<<endl;*/
				cout<<"Top "<<i<<" vertices done."<<endl;
				cout<<"Current label size (Bytes):"<<(label_est.inout_same+label_est.only_in+label_est.only_out)*6+label_est.inout_different*8<<endl;				
		}
		v_degreeit=v_degree.begin();	
		if(v_degreeit->second<=cut_degree)
		{
			gettimeofday(&after_time,NULL);
			double duration=(after_time.tv_sec - before_time.tv_sec)*1000.0 + (after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
			
			cout<<"Prelabeling time:"<<duration<<endl;
			cout<<"Estimated total label size (Bytes): "<<(label_est.inout_same+label_est.only_in+label_est.only_out)*6+label_est.inout_different*8<<endl;
			cout<<"Estimated total label size by in and out only (Bytes): "<<(label_est.inout_same*2+label_est.only_in+label_est.only_out+label_est.inout_different*2)*6<<endl;
			cout<<"Estimated total label size by in and out only which include self-vertex (Bytes): "<<(label_est.inout_same*2+label_est.only_in+label_est.only_out+label_est.inout_different*2+g.num_vertices()*2)*6<<endl;
			cout<<"Labels with same in and out: "<<label_est.inout_same<<endl;
			cout<<"Label with only in: "<<label_est.only_in<<endl;
			cout<<"Label with only out: "<<label_est.only_out<<endl;
			cout<<"Label with different in and out: "<<label_est.inout_different<<endl;
			cout<<"Total number of vertices processed:"<<i<<endl;
			
			
			ofstream l_in(LinFile, ios_base::out);
			ofstream l_out(LoutFile, ios_base::out);
			ofstream l_in_out_same(LinoutsameFile, ios_base::out);
			ofstream l_in_out_different(LinoutdifferentFile, ios_base::out);
			for(i=0;i<g.num_vertices();i++)
			{
				l_in<<l_in_labelsize[i]<<endl;
				l_out<<l_out_labelsize[i]<<endl;
				l_in_out_same<<l_in_out_same_labelsize[i]<<endl;
				l_in_out_different<<l_in_out_different_labelsize[i]<<endl;
			}
			l_in.close();
			l_out.close();
			l_in_out_same.close();
			l_in_out_different.close();
			break;
		}
		BFSLabel(g, v_degreeit->first, length_limit, label_est, l_in_labelsize,l_out_labelsize, l_in_out_same_labelsize, l_in_out_different_labelsize);//label for a center point with high degree
		vector<int>::iterator erase_iter;//erase the edge go to the center point
		vector<int>::iterator erase_it;
		for(erase_iter=g.graph[v_degreeit->first].outList.begin();erase_iter!=g.graph[v_degreeit->first].outList.end();erase_iter++)
		{
			int pos=0;
			for(erase_it=g.graph[*erase_iter].inList.begin();erase_it!=g.graph[*erase_iter].inList.end();erase_it++)
			{
				if(*erase_it==v_degreeit->first)
				{
					g.graph[*erase_iter].inList.erase(g.graph[*erase_iter].inList.begin()+pos);
					
					findit.first=*erase_iter;
					findit.second=vd[*erase_iter];
					updateit=v_degree.find(findit);
					degree.first=updateit->first;
					degree.second=updateit->second-1;
					vd[*erase_iter]=updateit->second-1;
					v_degree.erase(updateit);
					v_degree.insert(degree);
					break;
				}
				pos++;
			}
		}
		g.graph[v_degreeit->first].outList.clear();
		
		for(erase_iter=g.graph[v_degreeit->first].inList.begin();erase_iter!=g.graph[v_degreeit->first].inList.end();erase_iter++)
		{
			int pos=0;
			for(erase_it=g.graph[*erase_iter].outList.begin();erase_it!=g.graph[*erase_iter].outList.end();erase_it++)
			{
				if(*erase_it==v_degreeit->first)
				{
					g.graph[*erase_iter].outList.erase(g.graph[*erase_iter].outList.begin()+pos);
					
					findit.first=*erase_iter;
					findit.second=vd[*erase_iter];
					updateit=v_degree.find(findit);
					degree.first=updateit->first;
					degree.second=updateit->second-1;
					vd[*erase_iter]=updateit->second-1;
					v_degree.erase(updateit);
					v_degree.insert(degree);
					break;
				}
				pos++;
			}
		}
		g.graph[v_degreeit->first].inList.clear();
		v_degree.erase(v_degree.begin());
	}
	vector<int>::iterator edgeit;
	edge_num=0;
	int number=0;
	for(i=0;i<g.num_vertices();i++)
	{
		for(edgeit=g.graph[i].outList.begin();edgeit!=g.graph[i].outList.end();edgeit++)
		{
				edge_num++;
		}
		number=number+g.graph[i].outList.size();
	}
	
	cout<<"Total number of edges by counting after decentralization:"<<edge_num<<endl;
	cout<<"Total number of edges by summarizing after decentralization:"<<number<<endl;	
	
}



