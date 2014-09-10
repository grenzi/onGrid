#ifndef _UTILITYFUNCTION_H
#define __UTILITYFUNCTION_H

#include "Graph.h"


struct vertex_inqueue
{
	int num;
	int level;
	int hop;
};

struct Label
{
	int dest;
	int dist;
	int hop;
};


void BFSLabel(Graph& g, int v, int length_limit, labelEstPara& label_est, vector<int>& l_in_labelsize, vector<int>& l_out_labelsize, vector<int>& l_in_out_same_labelsize, vector<int>& l_in_out_different_labelsize);

#endif



