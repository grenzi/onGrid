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
};

class BFSsearch
{
public:

	void FindShortestPath(int begin, int end, int maxDepth, Graph& g, vector<int>& path);
	void FindMultiplePath(int begin, int end, int maxDepth, Graph& g, vector<vector<int> >& pathcollection);
	int FindMultiplePathNum(int begin, int end, int maxDepth, Graph& g);
	int FindShortestDist(int begin, int end, int maxDepth, Graph& g);
};
