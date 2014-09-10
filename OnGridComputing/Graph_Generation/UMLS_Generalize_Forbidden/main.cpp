#include "umls.h"


int main(int argc, char* argv[])
{
	umls u;
	char* filename=argv[1];
	char* MRSTYFile=argv[2];
	char* vforbiddenFile=argv[3];
	char* eforbiddenFile=argv[4];
	char* VertexSemanticLabelFile=argv[5];
	char* VertexFile=argv[6];
	char* EdgeinformationFile=argv[7];
	char* EdgeFile=argv[8];
	char* GraphFile=argv[9];	
	u.parseUMLS(filename, MRSTYFile, vforbiddenFile, eforbiddenFile, VertexSemanticLabelFile, VertexFile, EdgeinformationFile, EdgeFile, GraphFile);
}
