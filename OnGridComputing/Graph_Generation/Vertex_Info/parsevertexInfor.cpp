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

struct CUIInfo{
	string Language;
	string Infor;
};

int main(int argc, char* argv[])
{
	map<string,CUIInfo> vertex_info;//keep only the first preferred name for each CUI

	char* inMRCONSOfile=argv[1];
	ifstream in(inMRCONSOfile);
	if(!in)
	{
		cout<<"error!"<<endl;
	}

	string buf;	
	int number_of_line_processed=0;
	set<string> CompleteCUICollection;
	while(getline(in,buf))
	{
		number_of_line_processed++;
		string vertex=buf.substr(0, buf.find("|"));
		CompleteCUICollection.insert(vertex);
		
		buf.erase(0, buf.find("|")+1);
		
		string language=buf.substr(0, buf.find("|"));
		
		for(int i=1;i<6;i++)
		{
			buf.erase(0, buf.find("|")+1);
		}

		if(buf.substr(0, buf.find("|"))=="Y")
		{
			for(int i=0;i<8;i++)
			{
				buf.erase(0, buf.find("|")+1);
			}
			
			string infor=buf.substr(0, buf.find("|"));
			map<string,CUIInfo>::iterator iter_CUI=vertex_info.find(vertex);
			
			if(iter_CUI==vertex_info.end())
			{
				CUIInfo inf_tmp;
				inf_tmp.Language=language;
				inf_tmp.Infor=infor;
				vertex_info.insert(pair<string,CUIInfo>(vertex,inf_tmp));
			}
			else if (iter_CUI->second.Language!="ENG" && language=="ENG")//overwrite with English info
			{
				iter_CUI->second.Language=language;
				iter_CUI->second.Infor=infor;
			}
		}
	}

	char* outVertexLabelfile=argv[2];
	ofstream out(outVertexLabelfile);

	for(map<string,CUIInfo>::iterator it=vertex_info.begin();it!=vertex_info.end();it++)
	{
		out<<it->first<<"	"<<it->second.Infor<<endl;
	}
	
	cout<<"Number of line processed: "<<number_of_line_processed<<endl;
	cout<<"Total Number of CUIs in vertex info: "<<vertex_info.size()<<endl;
	cout<<"Total Number of CUIs in the original datasets: "<<CompleteCUICollection.size()<<endl;
	if (vertex_info.size()<CompleteCUICollection.size())
		cout<<"There are some CUIs without a preferred name."<<endl;
	cout<<outVertexLabelfile<<" successfully generated!"<<endl;

}
