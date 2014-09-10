#ifndef _GREEDYLABEL_H
#define _GREEDYLABEL_H

#include "Graph.h"
#include "Utilityfunction.h"
#include <cstring>
#include <hash_map.h>

//#define DEBUG
#define MAX 100000
#define UPPER 0
#define LOWER 1
#define BOTH  2


typedef pair<int,int> intpair;

struct intpaircomp{
	bool operator() (const intpair& p1, const intpair&p2) const{
		if(p1.second>p2.second)
			return true;
		else
		{
			if(p1.second<p2.second)
				return false;
			else
			{
				if(p1.first<p2.first)
					return true;
				else 
					return false;
			}
		}
	}
};




class Greedylabel {
	public:
		Graph& g;
		vector<vector<Label> > l_in, l_out;
		//float threshold;
	
		Greedylabel(Graph& graph);
		Greedylabel(Graph& graph, float Thresh);
		~Greedylabel();	
		void run(int length_limit, char* LinFile, char* LoutFile, char* LinoutsameFile, char* LinoutdifferentFile, char* sinksourcefile);
	/*	int test_reach(int src, int trg);
		void displaylabels();*/
};
#endif
