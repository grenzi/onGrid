#ifndef _UTILITYFUNCTION_H
#define __UTILITYFUNCTION_H

#include "Graph.h"


struct vertex_inqueue
{
	int num;
	int level;
	int offset;
};

struct Label
{
	unsigned char dest1;
	unsigned char dest2;
	unsigned char dest3;
	unsigned char port1;
	unsigned char port2;
	unsigned char distance;
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
{
	LabelSpecial():direction(-1), dest(-1), port(-1), distance(-1), ex_distance(-1){};
	int direction; //1 means incoming; 2 means outgoing; 3 means both through the same port; 
	int dest;
	int port;
	int distance;//used for incoming or outgoing distance when direction=1,2;
	int ex_distance;//(optional) used for outgoing distance when direction=3;
	uint16_t score_in;
	uint16_t score_out;
};

struct Match
{
	Match():distance(-1),out_offset(-1),in_offset(-1),common_dest(-1){};
	int distance;
	int out_offset;
	int in_offset;
	int common_dest;
	double pContribution;
	
	bool operator < (const Match& m) const {
		return (pContribution > m.pContribution);
	}
};

inline int BinarySearchOffset(vector<Label>& l, int goal)
{
	int offset;
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));
	int min=0;
	int max=l.size()-1;
	int mid;
	int index;
	while(true)
	{
		mid=min+(max-min)/2;
		index=int(l[mid].dest1)*pow16+int(l[mid].dest2)*pow8+int(l[mid].dest3);
		if(goal>index)
			min=mid+1;
		else if(goal<index)
			max=mid-1;
		else if(goal==index)
		{
			offset=int(l[mid].port1)*pow8+int(l[mid].port2);
			return offset;
		}
		if(min>max)
			return -1;//not found is allowed
	}
};

inline int BinarySearchOffset(vector<LabelDiff>& l, int goal, bool incoming)
{
	int offset;
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));
	int min=0;
	int max=l.size()-1;
	int mid;
	int index;
	while(true)
	{
		mid=min+(max-min)/2;
		index=int(l[mid].dest1)*pow16+int(l[mid].dest2)*pow8+int(l[mid].dest3);
		if(goal>index)
			min=mid+1;
		else if(goal<index)
			max=mid-1;
		else if(goal==index)
		{
			if (incoming)
				offset=int(l[mid].port1_in)*pow8+int(l[mid].port2_in);
			else
				offset=int(l[mid].port1_out)*pow8+int(l[mid].port2_out);
			return offset;
		}
		if(min>max)
			return -1;//not found is allowed
	}
};

/*
inline int Binaryfind(vector<Label> l, int goal)
{
	if(l.size()==0)
		return -1;
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));

	int min=0;
	int max=l.size()-1;
	int mid;
	int index;
	while(true)
	{
		mid=min+(max-min)/2;
		index=int(l[mid].dest1)*pow16+int(l[mid].dest2)*pow8+int(l[mid].dest3);
		if(goal>index)
			min=mid+1;
		else if(goal<index)
			max=mid-1;
		else if(goal==index)
		{
			return mid;
		}
		if(min>max)
		{
			return -1;
		}
	}
}*/
#endif



