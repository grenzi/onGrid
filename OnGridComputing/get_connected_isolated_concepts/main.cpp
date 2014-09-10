#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	struct timeval after_time, before_time;
	float query_time,construct_time;

	char* in_graph_file = argv[1];
	char* in_cui_file = argv[2];
	char* in_name_file = argv[3];
	char* out_isolated_file = argv[4];
	char* out_connected_file = argv[5];
	
	ifstream in_graph(in_graph_file);
	ifstream in_cui(in_cui_file);
	ifstream in_name(in_name_file);
	
	if(!in_graph || !in_cui || !in_name)	{
		cout<<"Error in opening at least one input file. Exit."<<endl;
		exit(0);
	}
	
	Graph g(in_graph);
	cout<<"Number of vertices: "<<g.num_vertices()<<endl;
	in_graph.close();

	int n=0;
	string buf;
	map<int, string> cui_map;
	while (getline(in_cui, buf)) {
		cui_map[n] = buf;
		n++;
	}
	in_cui.close();
	
	map<string, string> name_map;
	while (getline(in_name, buf)) {
		string cui = buf.substr(0, buf.find("	"));
		buf.erase(0, buf.find("	")+1);  // contains name
		name_map[cui] = buf;
	}
	in_name.close();
	
	// write isolated/connected in graph to file
	ofstream out_isolated(out_isolated_file);
	ofstream out_connected(out_connected_file);
	
	map<int, string>::iterator ait;
	map<string, string>::iterator bit;
	for (int i = 0; i < g.num_vertices(); i++) {
		ait = cui_map.find(i);
		if (ait == cui_map.end()) {
			cout << "could not find " << i << " in map." << endl;
			exit(0);
		}
		bit = name_map.find(ait->second);
		if (bit == name_map.end()) {
			cout << "could not find " << ait->second << " in map." << endl;
			exit(0);
		}
		
		if (g.out_degree(i) == 0 && g.in_degree(i) == 0) {
			out_isolated << "\t" << ait->second << "\t" << bit->second << endl;
		}
		else {
			out_connected << "\t" << i << "\t" << ait->second << "\t" << bit->second << endl;
		}
	}
	
	out_isolated.close();
	out_connected.close();
	cout << "done." << endl;
}
