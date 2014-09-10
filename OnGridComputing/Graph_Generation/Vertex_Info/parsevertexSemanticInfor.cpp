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

using namespace std;

int main(int argc, char* argv[])
{

	char* inMRSTYfile=argv[1];
	ifstream in(inMRSTYfile);
	if(!in)
	{
		cout<<"error!"<<endl;
	}

	string buf;	
	set<string> VSemantics;
	while(getline(in,buf))
	{
		for(int i=0;i<3;i++)
			buf.erase(0, buf.find("|")+1);

		VSemantics.insert(buf.substr(0, buf.find("|")));
	}

	char* outVertexSemanticLabelfile=argv[2];
	ofstream out(outVertexSemanticLabelfile);

	for(set<string>::iterator it=VSemantics.begin();it!=VSemantics.end();it++)
	{
		out<<*it<<endl;
	}
	
	cout<<outVertexSemanticLabelfile<<" successfullly generated!"<<endl;

}
