#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include "Greedylabel.h"
#include <sys/time.h>
using namespace std;

#define MAX 100000

void FloydWarshall(Graph g, vector<vector<int> >& TC)
{
	int i,j,k;
	int num_vertex;
	num_vertex= g.num_vertices();
		for(j=0;j<num_vertex;j++)
		{
			TC[j][j]=0;
		}
	vector<int>::iterator iter;
	for(i=0;i<num_vertex;i++)
	{
		for(iter=g.graph[i].outList.begin(); iter!=	g.graph[i].outList.end(); iter++)
		{
			TC[i][*iter]=1;
		}
	}
	for(k=0;k<num_vertex;k++)
	{
		for(i=0;i<num_vertex;i++)
			for(j=0;j<num_vertex;j++)
			{
				TC[i][j]=min(TC[i][j],TC[i][k]+TC[k][j]);
			}
		if(k%100==0)
		cout<<k<<endl;
	}
}



int main(int argc, char* argv[])
{
	struct timeval after_time, before_time;
	float query_time,construct_time;
	int i;
	vector<vector<int> > TC;
	char* filename;
	//filename="E:/medical project/dataset_sigmod08/dataset_sigmod08/real_data/UndirectedGraph06.gra";
	filename=argv[1];
	int length_limit=atoi(argv[2]);
	char* LinFile=argv[3];
	char* LoutFile=argv[4];
	char* LinoutsameFile=argv[5];
	char* LinoutdifferentFile=argv[6];
	char* sinksourcefile=argv[7];
	ifstream infile(filename);
	if(!infile)
	{
		cout<<"error!"<<endl;
	}
	Graph g(infile);
	cout<<g.num_vertices()<<endl;
	infile.close();
	cout<<"***********************"<<endl;
	Greedylabel t=Greedylabel(g);
/*	int length_limit;
	int batch_size;
	cout<<"length_limit:"<<endl;
	cin>>length_limit;
	cout<<"batch_size:"<<endl;
	cin>>batch_size;*/
	gettimeofday(&before_time,NULL);
	t.run(length_limit, LinFile, LoutFile, LinoutsameFile, LinoutdifferentFile, sinksourcefile);
	gettimeofday(&after_time,NULL);
	construct_time=(after_time.tv_sec - before_time.tv_sec)*1000.0 + (after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
	cout<<"Total preconstruct time:"<<construct_time<<endl;

	
}
