#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include <queue>


typedef list<int> path;        //the path
typedef set<int> vinpath;      //the vertices in the path

struct node_info
{
	node_info(): parent(-1), level(-1),visited(false){};
	int parent;
	int level;
	bool visited;
	uint16_t score_in;
	uint16_t score_out;
};

class BFSsearch
{
public:

	void FindShortestPath(int begin, int end, int maxDepth, Graph& g, vector<int>& path);
	void FindMultiplePath(int begin, int end, int maxDepth, Graph& g, vector<vector<int> >& pathcollection);
	int FindMultiplePathNum(int begin, int end, int maxDepth, Graph& g);
	int FindShortestDist(int begin, int end, int maxDepth, Graph& g);
	int FindShortestDist(const int& begin, const int& end, const int& maxDepth, const Graph& g, int& score_in);
};
