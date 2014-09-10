#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <deque>
#include <algorithm>
#include <utility>
#include <cmath>
//#include <hash_map.h>

using namespace std;

#define MAX_VAL 10000000
#define MIN_VAL -10000000

struct Vertex {
	Vertex():visited(false), fat(false), topo_id(-1), path_id(-1), dfs_order(-1), pre_order(-1), post_order(-1), first_visit(0){}; 
	bool visited;
	bool fat;	// fat node
	int topo_id;	// topological order
	int path_id;	// path id
	int dfs_order;
	int pre_order;	
	int post_order;
	int first_visit; // for test
	int rank;
};


typedef vector<int> EdgeList;	// edge list represented by vertex id list
typedef vector<Vertex> VertexList;	// vertices list (store real vertex property) indexing by id

struct In_OutList {
	EdgeList inList;
	EdgeList outList;
	vector<int> portList;
};
typedef vector<In_OutList> GRA;	// index graph

class Graph {
	public:
		GRA graph;
		VertexList vl;
		int vsize;
		
	
		Graph();
		Graph(int);
		Graph(GRA&, VertexList&);
		Graph(istream&, istream&);
		~Graph();
		void split_vertex_rank(vector<string>&, const string&, const char);
		void parse_rank_list(map<int, int>&, istream&);
		void readGraph(istream&, istream&);
		void writeGraph(ostream&);
		void printGraph();
		void addVertex(int, int);
		void addEdge(int, int, int, int);
		int num_vertices();
		int num_edges();
		int get_rank(int) const;
		VertexList& vertices();
		EdgeList& out_edges(int);
		EdgeList& in_edges(int);
		int out_degree(int);
		int in_degree(int);
		vector<int> getRoots();
		bool hasEdge(int, int);	
		Graph& operator=(const Graph&);
		Vertex& operator[](const int);

	/*	Graph(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist);
		void extract(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist);
		void printMap(hash_map<int,vector<int> >& inlist, hash_map<int,vector<int> >& outlist);*/

	private:
		void RebuildLists();		
};	

#endif
