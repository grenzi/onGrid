#ifndef _UMLS_H
#define _UMLS_H

//1   5    8

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <algorithm>
#include <utility>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>


using namespace std;

class umls
{
	public:
		void parseUMLS(char* filename, char* MRSTYFile, char* vforbiddenFile, char* eforbiddenFile, char* VertexSemanticLabelFile, char* VertexFile, char* EdgeinformationFile, char* EdgeFile, char* GraphFile);
		//char* vertexFile;
		//char* EdgeinformationFile;
		//char* EdgeFile;
		//char* GraphFile;

		umls();
		//umls(char* f1,char* f2, char* f3,char* f4);
		~umls();
		
		
};


#endif

