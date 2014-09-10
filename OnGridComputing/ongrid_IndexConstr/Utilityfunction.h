#ifndef _UTILITYFUNCTION_H
#define __UTILITYFUNCTION_H

#include "Graph.h"


struct vertex_inqueue
{
	int num;
	int level;
	uint16_t score_in;
	uint16_t score_out;
};

struct Label
{
	unsigned char dest1;
	unsigned char dest2;
	unsigned char dest3;
	unsigned char port1;
	unsigned char port2;
	unsigned char distance;//split into two parts (upper for in, lower for out) when in and out through the same port
	uint16_t score_in;
	uint16_t score_out;
};

struct LabelDiff
{
	unsigned char dest1;
	unsigned char dest2;
	unsigned char dest3;
	unsigned char port1_in;
	unsigned char port2_in;
	unsigned char port1_out;
	unsigned char port2_out;	
	unsigned char distance;//split into two parts (upper for in, lower for out)
	uint16_t score_in;
	uint16_t score_out;
};

struct LabelSpecial
{//index takes 3 bytes
	LabelSpecial():direction(-1), dest(-1), port(-1), distance(-1), ex_distance(-1), score_in(-1), score_out(-1){};
	int direction; //1 means incoming; 2 means outgoing; 3 means both through the same port; 1 byte is enough;
	int dest;//3-byte is enough;
	int port;//3-byte is enough;
	int distance;//used for incoming or outgoing distance when direction=1,2; 2-byte is enough; 
	int ex_distance;//(optional) used for outgoing distance when direction=3; 2-byte is enough;
	uint16_t score_in;
	uint16_t score_out;
};//14 bytes total at the minimum; or 24 bytes if simply use int for all.

void BFSLabel(Graph& g, int v, int length_limit, labelEstPara& label_est, vector<int>& l_in_labelsize, vector<int>& l_out_labelsize, vector<int>& l_in_out_same_labelsize, vector<int>& l_in_out_diff_labelsize, vector<vector<Label> >& l_in, vector<vector<Label> >& l_out, vector<vector<Label> >& l_in_out_same, vector<vector<LabelDiff> >& l_in_out_diff, multimap<int, LabelSpecial>& l_special);

#endif



