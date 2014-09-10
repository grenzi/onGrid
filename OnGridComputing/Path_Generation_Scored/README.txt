actual path query, contains shortest path and multiple path

a. compile:
	make
	
b. run:
./kDLSPath [input files] length_limit [label files] [query file and result file]

c. for example
./kDLSPath ../../Datasets/graph_Full_Forbid.gra ../../Datasets/vertexCUI_Full.txt ../../Datasets/VertexLabelFile_Full.txt ../../Datasets/EdgeIDFile_Full.txt ../../Datasets/EdgeLabelFile_Full.txt 6 ../../Datasets/sinksource_Full_Forbid_file.txt ../../Datasets/Lin_Full_Forbid_Label_6 ../../Datasets/Lout_Full_Forbid_Label_6 ../../Datasets/Linoutsame_Full_Forbid_Label_6 ../../Datasets/Linoutdiff_Full_Forbid_Label_6 ../../Datasets/Lspecial_Full_Forbid_Label_6 ./queryfile.txt ./result.txt