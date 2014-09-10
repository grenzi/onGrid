#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include "Greedylabel.h"
//#include "BFS.h"
#include <sys/time.h>
#include <algorithm>
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

struct Labelcomp
{
	bool operator() (const Label& l1, const Label& l2) const{
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

struct LabelDiffcomp
{
	bool operator() (const LabelDiff& l1, const LabelDiff& l2) const{
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
} mylabeldiffcomp;


int main(int argc, char* argv[])
{
	struct timeval after_time, before_time;
	float query_time,construct_time;
	int i;
	vector<vector<int> > TC;
	char* filename=argv[1];
	int length_limit=atoi(argv[2]);
	char* LinFile=argv[3];
	char* LoutFile=argv[4];
	char* LinoutsameFile=argv[5];
	char* LinoutdifferentFile=argv[6];
	char* in_label=argv[7];
	char* out_label=argv[8];
	char* in_out_same_label=argv[9];
	char* in_out_diff_label=argv[10];
	char* special_label=argv[11];//to record few offset overflow cases.
	char* vertex_ranking = argv[12];

	ifstream infile(filename);
	ifstream inrankfile(vertex_ranking);
	if(!infile || !inrankfile)
	{
		cout<<"error!"<<endl;
		exit(0);
	}
	Graph g(infile, inrankfile);
	cout<<"Number of vertices:"<<g.num_vertices()<<endl;

	
	infile.close();
	inrankfile.close();


	cout<<"***********************"<<endl;
	Greedylabel t=Greedylabel(g);

	gettimeofday(&before_time,NULL);
	t.run(length_limit, LinFile, LoutFile, LinoutsameFile, LinoutdifferentFile);
	
	ofstream lin(in_label, ios_base::out);
	ofstream lout(out_label, ios_base::out);
	ofstream linoutsame(in_out_same_label, ios_base::out);	
	ofstream linoutdiff(in_out_diff_label, ios_base::out);
	ofstream lspecial(special_label, ios_base::out);
	
	for(i=0;i<g.num_vertices();i++)
	{
		//cout<<i<<endl;
		sort(t.l_in[i].begin(),t.l_in[i].end(),mylabelcomp);
		for(vector<Label>::iterator initer=t.l_in[i].begin();initer!=t.l_in[i].end();initer++)
		{
			lin<<int(initer->dest1)<<"	"<<int(initer->dest2)<<"	"<<int(initer->dest3)<<"	"<<int(initer->port1)<<"	"<<int(initer->port2)<<"	"<<int(initer->distance)<<"	"<<initer->score_in<<"	"<<initer->score_out<<"	";
		}
		lin<<endl;
		
		sort(t.l_out[i].begin(),t.l_out[i].end(),mylabelcomp);
		for(vector<Label>::iterator outiter=t.l_out[i].begin();outiter!=t.l_out[i].end();outiter++)
		{
			lout<<int(outiter->dest1)<<"	"<<int(outiter->dest2)<<"	"<<int(outiter->dest3)<<"	"<<int(outiter->port1)<<"	"<<int(outiter->port2)<<"	"<<int(outiter->distance)<<"	"<<outiter->score_in<<"	"<<outiter->score_out<<"	";
		}
		lout<<endl;

		sort(t.l_in_out_same[i].begin(),t.l_in_out_same[i].end(),mylabelcomp);
		for(vector<Label>::iterator initer=t.l_in_out_same[i].begin();initer!=t.l_in_out_same[i].end();initer++)
		{
			linoutsame<<int(initer->dest1)<<"	"<<int(initer->dest2)<<"	"<<int(initer->dest3)<<"	"<<int(initer->port1)<<"	"<<int(initer->port2)<<"	"<<int(initer->distance)<<"	"<<initer->score_in<<"	"<<initer->score_out<<"	";
		}
		linoutsame<<endl;		

		sort(t.l_in_out_diff[i].begin(),t.l_in_out_diff[i].end(),mylabeldiffcomp);
		for(vector<LabelDiff>::iterator initer=t.l_in_out_diff[i].begin();initer!=t.l_in_out_diff[i].end();initer++)
		{
			linoutdiff<<int(initer->dest1)<<"	"<<int(initer->dest2)<<"	"<<int(initer->dest3)<<"	"<<int(initer->port1_in)<<"	"<<int(initer->port2_in)<<"	"<<int(initer->port1_out)<<"	"<<int(initer->port2_out)<<"	"<<int(initer->distance)<<"	"<<initer->score_in<<"	"<<initer->score_out<<"	";
		}
		linoutdiff<<endl;	
	}
	
	for(multimap<int, LabelSpecial>::iterator speciter=t.l_special.begin(); speciter!=t.l_special.end(); speciter++)
	{
		lspecial<<speciter->first<<"	"<<speciter->second.direction<<"	"<<speciter->second.dest<<"	"<<speciter->second.port<<"	"<<speciter->second.distance<<"	"<<speciter->second.ex_distance<<"	"<<speciter->second.score_in<<"	"<<speciter->second.score_out<<"	";
	}
	lspecial<<endl;

	gettimeofday(&after_time,NULL);
	construct_time=(after_time.tv_sec - before_time.tv_sec)*1000.0 + (after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
	cout<<"Total Construct time:"<<construct_time<<endl;	

}
