Graph Generating: Reading the original data£¬Processing reverse_isa and generating the graph

a. source code
   main.cpp, umls.cpp, umls.h
b. compile: 
   make
c. output file
   graph_full:the graph file
   EdgeFile: the edge file, contain the edges, each line has three integer, the first two integer is the two nodes of the edge, the last integer is the edge information
   EdgeinformationFile: the edge information file
   vertexFile: the vertex file

d. Command Format:
./GraphGenerator [inputfile(graph)] [ForbiddenEdgeFile] [outputfiles]

For example:

./GraphGenerator ../../Datasets/MRREL_Full.RRF ../../Datasets/MRSTY_Full.RRF ./vForbidden_Full.txt ./eForbidden_Full.txt ../../Datasets/VertexSemanticLabelFile_Full.txt ../../Datasets/vertexCUI_Full.txt ../../Datasets/EdgeLabelFile_Full.txt ../../Datasets/EdgeIDFile_Full.txt ../../Datasets/graph_Full_Forbid.gra
