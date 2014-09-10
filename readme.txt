onGrid (Version 2.0 beta, maintained by Aaron Albin, Email: albin.9@osu.edu)

This is the latest version of the onGrid system (Aaron Albin, Xiaonan Ji, Tara B. Borlawsky, Zhan Ye, Simon Lin, Philip R.O. Payne, Kun Huang, Yang Xiang, "Enabling efficient online studies on conceptual relationships between medical terms", JMIR Medical Informatics, In press).
The system is currently available online at https://netlab.bmi.osumc.edu/ongrid.

Software License Agreement: You may use or modify this source code for research purposes, provided that you properly cite our paper in publication. This source code is provided on an as is basis and there is no guarantee on the program nor additional support offered. Neither the author(s) nor their institute(s) is liable under any circumstances. This archive (including this license agreement) may be updated without further notice.

Here is the general overview of files contained in this project:
	OnGridComputing: files necessary for generating the graph index files for onGrid.
	OnGridWebservice: files necessary for setting up the web-based onGrid system.
	
Instructions for OnGridComputing:
1. Graph_Generation (Vertex_Info, UMLS_Generatlize_Forbidden)
	Create the UMLS graph.
2. UMLS_Semantic_Ranking
	Get rank scores for semantic types
3. Vertex_Ranking_Constr
	Assign rank score for each CUI in a file
4. ongrid_IndexPreConstr, ongrid_IndexConstr
	Generate label files
5. Path_Generation_Scored
	Demonstrates source code for querying paths from the command-line
6. get_connected_isolated_concepts
	Get isolated/connected vertices to be input into the database

Instructions for OnGridWebservice:
1. A database must be setup similar to that described in OnGridWebservice/db_structure.sql.
2. html and cgi-bin files should be placed into respective directories on the hosting server.
3. Connection strings for the database should be modified in:
	html/connect.php
	cgi-bin/LDPWebservice/main.cpp
	cgi-bin/OnGridWebservice/main.cpp
4. The index files generated from OnGridComputing are loaded in cgi-bin/OnGridWebservice/main.cpp.  The UMLS file MRCONSO.RFF is needed for cgi-bin/LDPWebservice/main.cpp.
5. Upload and download directories should be modified in both cgi-bin/OnGridWebservice and html. The placeholders are:
	"matrix_temporary_uploads", "matrix_temporary_results", "matrix_uploads", "matrix_results"
6. cgi-bin/OnGridWebservice/scripts/cleanup.py should point to the upload and download directories to cleanup files left in those directories. It can be ran manually or added to the servers iptables for periodic execution.
7. For the user file upload feature, an email address is needed for sending and verifying user emails. Files should be modified in cgi-bin/OnGridWebservice and html.
8. onGrid and LDPMap should be ran as two separate background processes on the server using nohup. These periodically check their respective tables in the database for tasks to perform. Once the result is calculated, the respective table is updated. The result is then used by the calling php files.

