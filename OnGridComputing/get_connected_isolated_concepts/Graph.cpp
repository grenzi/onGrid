
#include "Graph.h"

Graph::Graph() {
	graph = GRA();
	vl = VertexList();
}

Graph::Graph(int size) {
	vsize = size;
	vl = VertexList(size);
	graph = GRA(size, In_OutList());
}

Graph::Graph(GRA& g, VertexList& vlist) {
	vsize = vlist.size();
	graph = g;
	vl = vlist;
}

Graph::Graph(istream& in) {
	readGraph(in);
}

Graph::~Graph() {}

void Graph::printGraph() {
	writeGraph(cout);
}

void Graph::readGraph(istream& in) {
	string buf;
	getline(in, buf);

	if (buf != "graph_for_greach") {
		cout << "BAD FILE FORMAT!" << endl;
		exit(0);
	}
	
	int n;
	getline(in, buf);
	istringstream(buf) >> n;
	// initialize
	vsize = n;
	vl = VertexList(n);
	graph = GRA(n, In_OutList());	

	for (int i = 0; i < n; i++)
		addVertex(i);

	string sub;
	int idx;
	int sid = 0;
	int tid = 0;
	while (getline(in, buf)) {
		idx = buf.find(":");
		buf.erase(0, idx+2);
		while (buf.find(" ") != string::npos) {
			sub = buf.substr(0, buf.find(" "));
			istringstream(sub) >> tid;
			buf.erase(0, buf.find(" ")+1);
			addEdge(sid, tid);
		}
		++sid;
		if(sid%100==0)
		{
		//	cout<<"999"<<endl;
		}
	}
	RebuildLists();
}	

void Graph::RebuildLists(){
//IMPORTANT: Assume inList and outList are in ascending order before processing. 
//After processing, the ascending orders of inList and outList do not hold. 
//The first part consists of vertices in both inList and outList.
//The second part consists of vertices in either inList or outList.
//The portList is NOT in order.
	bool symmetric_test=true;
	for(int i=0; i<vl.size(); i++)
	{
		vector<int>::iterator in_iter=graph[i].inList.begin();
		vector<int>::iterator out_iter=graph[i].outList.begin();
		vector<int> only_in;
		vector<int> only_out;
		while(in_iter!=graph[i].inList.end() && out_iter!=graph[i].outList.end())
		{
			if(*in_iter==*out_iter){
				graph[i].portList.push_back(*in_iter);
				in_iter++;
				out_iter++;
			}else if (*in_iter<*out_iter){
				only_in.push_back(*in_iter);
				in_iter++;
			}else{//i.e., *in_iter>*out_iter
				only_out.push_back(*out_iter);
				out_iter++;
			}
		}
		if(in_iter!=graph[i].inList.end())//i.e., out_iter==graph[i].outList.end()
		{
			only_in.insert(only_in.end(),in_iter,graph[i].inList.end());
			symmetric_test=false;
		}
		else if (out_iter!=graph[i].outList.end())//i.e., in_iter==graph[i].inList.end()
		{
			only_out.insert(only_out.end(),out_iter,graph[i].outList.end());
			symmetric_test=false;
		}
		
		//rebuild inList and outList
		graph[i].inList=graph[i].portList;
		graph[i].outList=graph[i].portList;
		graph[i].inList.insert(graph[i].inList.end(),only_in.begin(),only_in.end());
		graph[i].outList.insert(graph[i].outList.end(),only_out.begin(),only_out.end());
		
		//completing portList
		graph[i].portList.insert(graph[i].portList.end(),only_in.begin(),only_in.end());
		graph[i].portList.insert(graph[i].portList.end(),only_out.begin(),only_out.end());
	}
	if(symmetric_test==true)
		cout<<"The graph is symmetric and may be considered as an undirected graph"<<endl;
	else
		cout<<"The graph is asymmetric and cannot be considered as an undirected graph"<<endl;
}

void Graph::writeGraph(ostream& out) {
	cout << "Graph size = " << graph.size() << endl;
	out << "graph_for_greach" << endl;
	out << vl.size() << endl;

	GRA::iterator git;
	EdgeList el;
	EdgeList::iterator eit;
	VertexList::iterator vit;
	int i = 0;
	for (i = 0; i < vl.size(); i++) {
		out << i << ": ";
		el = graph[i].outList;
		for (eit = el.begin(); eit != el.end(); eit++)
			out << (*eit) << " ";
		out << "#" << endl;
	}
/*
	cout << "** In List for graph **" << endl;
	for (i = 0; i < vl.size(); i++) {
		out << i << ": ";
		el = graph[i].inList;
		for (eit = el.begin(); eit != el.end(); eit++)
			out << (*eit) << " ";
		out << "#" << endl;
	}
*/
}

void Graph::addVertex(int vid) {
	if (vid >= vl.size()) {
		int size = vl.size();
		for (int i = 0; i < (vid-size+1); i++) {
			graph.push_back(In_OutList());
			vl.push_back(Vertex());
		}
		vsize = vl.size();
	}

	Vertex v;
	v.visited = false;
	vl[vid] = v;

	EdgeList il = EdgeList();
	EdgeList ol = EdgeList();
	In_OutList oil = In_OutList();
	oil.inList = il;
	oil.outList = ol;
	graph[vid] = oil;	
}

void Graph::addEdge(int sid, int tid) {
	if (sid >= vl.size())
		addVertex(sid);
	if (tid >= vl.size())
		addVertex(tid);
	// update edge list
	graph[tid].inList.push_back(sid);
	graph[sid].outList.push_back(tid);
}

int Graph::num_vertices() const {
	return vl.size();
}

int Graph::num_edges() {
	EdgeList el;
	GRA::iterator git;
	int num = 0;
	for (git = graph.begin(); git != graph.end(); git++) {
		el = git->outList;
		num += el.size();
	}
	return num;
}

// return out edges of specified vertex
EdgeList& Graph::out_edges(int src) {
	return graph[src].outList;
}

// return in edges of specified vertex
EdgeList& Graph::in_edges(int trg) {
	return graph[trg].inList;
}

int Graph::out_degree(int src) {
	return graph[src].outList.size();
}

int Graph::in_degree(int trg) {
	return graph[trg].inList.size();
}

// get roots of graph (root is zero in_degree vertex)
vector<int> Graph::getRoots() {
	vector<int> roots;
	GRA::iterator git;
	int i = 0;
	for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
		if (git->inList.size() == 0)
			roots.push_back(i);
	}
	
	return roots;
}

// check whether the edge from src to trg is in the graph
bool Graph::hasEdge(int src, int trg) {
	EdgeList el = graph[src].outList;
	EdgeList::iterator ei;
	for (ei = el.begin(); ei != el.end(); ei++)
		if ((*ei) == trg)
			return true;
	return false;

}

// return vertex list of graph
VertexList& Graph::vertices() {
	return vl;
}

Graph& Graph::operator=(const Graph& g) {
	if (this != &g) {
		graph = g.graph;
		vl = g.vl;
		vsize = g.vsize;
	}
	return *this;
}

// get a specified vertex property
Vertex& Graph::operator[](const int vid) {
	return vl[vid];
}

