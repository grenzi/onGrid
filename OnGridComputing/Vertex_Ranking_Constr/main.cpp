#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

void split(const string &s, const char& delim, vector<string>& tokens) {
    stringstream ss(s);
    string tok;
    while (getline(ss, tok, delim)) {
        tokens.push_back(tok);
    }
}

void clean_line(string& s) {
	if (!s.empty() && s[s.size() - 1] == '\r') {
		s.erase(s.size() - 1);
	}
}

void read_rank_map(const string& infile, map<string, int>& rank_map) {
	ifstream in(infile.c_str());
	if (!in) {
		cout << "Error reading file: " << infile << endl;
		exit(0);
	}
	
	string line;
	while (getline(in, line)) {
		clean_line(line);
		if (line != "") {
			vector<string> tokens;
			split(line, '\t', tokens);
			rank_map.insert(pair<string, int>(tokens[0], atoi(tokens[1].c_str())));
		}
	}
	in.close();
}

void read_cui_list(const string& infile, vector<string>& cui_list) {
	ifstream in(infile.c_str());
	if (!in) {
		cout << "Error reading file: " << infile << endl;
		exit(0);
	}
	
	string line;
	while (getline(in, line)) {
		clean_line(line);
		if (line != "") {
			cui_list.push_back(line);
		}
	}
	in.close();
}

void read_mrsty_map(const string& infile, multimap<string, string>& mrsty_map) {
	ifstream in(infile.c_str());
	if (!in) {
		cout << "Error reading file: " << infile << endl;
		exit(0);
	}
	
	string line;
	while (getline(in, line)) {
		clean_line(line);
		if (line != "") {
			vector<string> tokens;
			split(line, '|', tokens);
			mrsty_map.insert(pair<string, string>(tokens[0], tokens[3]));
		}
	}
	in.close();
}

int main (int argc, char* argv[]) {
	string inMRSTYFile = argv[1];
	string inVertexCUIFile = argv[2];
	string inSemanticRankingFile = argv[3];
	string outVertexFile = argv[4];

	// read semantic ranking into a map
	map<string, int> rank_map;
	read_rank_map(inSemanticRankingFile, rank_map);
	
	// read cui list
	vector<string> cui_list;
	read_cui_list(inVertexCUIFile, cui_list);
	
	// MRSTY file
	multimap<string, string> mrsty_map;
	read_mrsty_map(inMRSTYFile, mrsty_map);
	
	int index = 0;
	vector<string>::iterator ait;
	multimap<string, string>::iterator bit;
	pair<multimap<string,string>::iterator, multimap<string,string>::iterator> multi_it;
	map<string, int>::iterator rit;
	
	ofstream out(outVertexFile.c_str());
	for (ait = cui_list.begin(); ait != cui_list.end(); ait++) {
		int rank = 100;
		multi_it = mrsty_map.equal_range(*ait);
		for (bit = multi_it.first; bit != multi_it.second; bit++) {
			rit = rank_map.find(bit->second);
			if (rit != rank_map.end()) {
				int temp = rit->second;
				if (temp < rank)
					rank = temp;
			}
		}
		out << index << "\t" << rank << endl;

		index++;
	}
	out.close();
	
	return 0;
}
