#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "BFS.h"
#include "Graph.h"
#include "Greedylabel.h"
#include "DBManager.h"
#include "Log.h"
#include "Util.h"
using namespace std;


#define MAX 100000
#define QUERY_NUM 10000

typedef pair<int,int> vertexPair;

// global variables
Log logger;
pthread_mutex_t mutex;
Graph g;
Greedylabel kDLS(g);
set<int> sinksourcenodes;
vector<string> VERTEX; 
map<string,string> vertex_label;  //map a vertex to a meaning
multimap<vertexPair,int> edgeList;
vector<string> edgeLabelList;
set<int> forbiddenEdgeIDList;
map<string, int> vertexCUI_ID;  // map concepts to integer index, added for matrix implementation
int length_limit;
double EXPBASE;

struct sched_param sched_param = {
    sched_get_priority_min(SCHED_BATCH)
};

struct job_data {
	int job_id;
	int slot_id;
};

struct path_result {
	int distance;
	vector<vector<int> > allshortestpath;
};

struct path_data {
	string path_str;
	int path_length;
};

struct rcInfo{
	int ID;
	string preferredName;
};

int set_thread_to_core(const long tid, const int &core_id) {
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   cout << "number of cores: " << Util::numToStr(num_cores) << endl;
   if (core_id > num_cores) {
   	return -2;
   }
   cpu_set_t mask;
   CPU_ZERO(&mask);
   CPU_SET(core_id, &mask);
  
   return pthread_setaffinity_np(tid, sizeof(mask), &mask);
}

int process_distance(const int &ss, const int &tt) {
	bool s_is_sinksource;
	bool t_is_sinksource;
	
	BFSsearch _bfs;
	
	int querypair_sinksource = 0;
	int dist;
		
	if (sinksourcenodes.find(ss) != sinksourcenodes.end()) {
		s_is_sinksource=true;
	}
	else {
		s_is_sinksource = false;
	}
	
	if (sinksourcenodes.find(tt) != sinksourcenodes.end()) {
		t_is_sinksource=true;
	}
	else {
		t_is_sinksource = false;
	}
	
	if (s_is_sinksource || t_is_sinksource) {
		dist = _bfs.FindShortestDist(ss, tt, length_limit, g);
		querypair_sinksource++;
	}
	else {
		Match sdist_match = kDLS.FindShortestDist(ss, tt);
		dist=sdist_match.distance;
	}
	
	return dist;
}

int build_contribution_matrix(const vector<pair<string, rcInfo> >& row, const vector<pair<string, rcInfo> >& col, const string &special_key, const bool &email_exists) {

	std::size_t	x, max;

	string contributionFile = "";
	if (email_exists) {
		contributionFile = "./matrix_results/" + special_key + string("_contribution.txt");
	}
	else {
		contributionFile = "./matrix_temporary_results/" + special_key + string("_contribution.txt");
	}
	ofstream contributionMatrix(contributionFile.c_str());
	
	if(!contributionMatrix) {
		cout<<"Error in creating contribution output file. Exit."<<endl;
		return 0;
	}	
	
	// initially print out column headers
	int k = 0;
	for (vector<pair<string, rcInfo> >::const_iterator qq = col.begin(); qq!=col.end(); qq++, k++) {
		contributionMatrix << "\t" << qq->first;
	}
	contributionMatrix << endl;
	int sinksourceentries=0;//test
	int i=0;
	for (vector<pair<string, rcInfo> >::const_iterator ait=row.begin();ait!=row.end(); ait++, i++) {
		bool initial=true;

		// print out row header
		contributionMatrix << ait->first << "\t";
		
		int j=0;
		for (vector<pair<string, rcInfo> >::const_iterator bit = col.begin(); bit!=col.end(); bit++, j++) {
			double pContribution=0;
			int dist_a2b, dist_b2a;
			int a2b_score_in = 0;
			int b2a_score_in = 0;
			int allpathnumatob;
			int allpathnumbtoa;
			if (ait->second.ID == -1 || bit->second.ID == -1) {
				pContribution=0;
				allpathnumatob=0;
				allpathnumbtoa=0;
			}
			else if ((sinksourcenodes.find(ait->second.ID) != sinksourcenodes.end()) || (sinksourcenodes.find(bit->second.ID) != sinksourcenodes.end())) {
				sinksourceentries++;//test
				BFSsearch _bfs;
				dist_a2b = _bfs.FindShortestDist(ait->second.ID, bit->second.ID, length_limit, g, a2b_score_in);
				dist_b2a = _bfs.FindShortestDist(bit->second.ID, ait->second.ID, length_limit, g, b2a_score_in);
				
				if (dist_a2b <= length_limit && dist_a2b > 1) {
					allpathnumatob = 1;
					//pContribution = pContribution+  1.0 / double(pow(EXPBASE, double(dist_a2b - 1)));
					pContribution=pContribution + 1.0/a2b_score_in * (1.0/double(pow(EXPBASE, double(dist_a2b-1))));
				}
				else
					allpathnumatob=0;
				
				if (dist_b2a <= length_limit && dist_b2a > 1) {
					allpathnumbtoa=1;
					//pContribution = pContribution + 1.0 / double(pow(EXPBASE, double(dist_b2a - 1)));
					pContribution=pContribution + 1.0/b2a_score_in * (1.0/double(pow(EXPBASE,double(dist_b2a-1))));
				}
				else
					allpathnumbtoa = 0;
				}
			else {				
				allpathnumatob = kDLS.TotalTransitivePathNum_Contribution(ait->second.ID, bit->second.ID, pContribution, EXPBASE, a2b_score_in);
				allpathnumbtoa = kDLS.TotalTransitivePathNum_Contribution(bit->second.ID, ait->second.ID, pContribution, EXPBASE, b2a_score_in);
			}
			
			
			if(initial) {
				contributionMatrix << pContribution;
				initial = false;
			}
			else {
				contributionMatrix << "	" << pContribution;
			}
		}
		initial=true;
		contributionMatrix << endl;
	}

	contributionMatrix.close();
	cout << "Number of sinksource entries (i.e., src or trg is in the sinksource file) among src trg pairs: " << sinksourceentries << endl;
	
	return 1;
}

int build_distance_matrix(const vector<pair<string, rcInfo> >& row, const vector<pair<string, rcInfo> >& col, const string &special_key, const bool &email_exists) {
	
	std::size_t	x, max;

	string distanceFile = "";
	if (email_exists) {
		distanceFile = "./matrix_results/" + special_key + string("_distance.txt");
	}
	else {
		distanceFile = "./matrix_temporary_results/" + special_key + string("_distance.txt");	
	}
	ofstream distanceMatrix(distanceFile.c_str());
	
	if(!distanceMatrix) {
		cout<<"Error in creating distance output file. Exit."<<endl;
		return 0;
	}	
	
	int ss,tt;
	int querypair_sinksource=0;
	
	// initially print out column headers
	int k = 0;
	for (vector<pair<string, rcInfo> >::const_iterator qq = col.begin(); qq!=col.end(); qq++, k++) {
		distanceMatrix << "\t" << qq->first;
	}
	distanceMatrix << endl;
	int i = 0;
	for(vector<pair<string, rcInfo> >::const_iterator ait=row.begin();ait!=row.end(); ait++, i++) {
		bool initial=true;
		
		// print out row header
		distanceMatrix << ait->first << "\t";

		int j = 0;
		for(vector<pair<string, rcInfo> >::const_iterator bit=col.begin();bit!=col.end();bit++, j++) {
		
			ss = ait->second.ID;
			tt = bit->second.ID;
			int dist;
			if (ait->second.ID == -1 || bit->second.ID == -1) {
				dist=MAX_VAL;
			}else{
				dist = process_distance(ss, tt);
			}
			if(initial) {
				distanceMatrix << dist;
				initial = false;
			}
			else {
				distanceMatrix << "	" << dist;
			}
			
		}
		initial=true;
		distanceMatrix << endl;
	}
	
	distanceMatrix.close();
	return 1;
}

int build_knowledge_matrices(const vector<pair<string, rcInfo> >& row, const vector<pair<string, rcInfo> >& col, const string &special_key, const bool &email_exists) {
	int d_result = build_distance_matrix(row, col, special_key, email_exists);
	int c_result = build_contribution_matrix(row, col, special_key, email_exists);
	if ((c_result == 1) && (d_result == 1)) {
		return 1;
	}
	else
		return 0;
}

// path_data.path_str has the form:
// CUI:label	edgelabel	CUI:label	...
path_data pathdecoder(vector<int>& path) {
	// store in struct
	path_data pdata;
	pdata.path_length = path.size()-1;
	string v_CUI=VERTEX[path[0]];
	
	map<string,string>::iterator iter_vLabel=vertex_label.find(v_CUI);
	
	if(iter_vLabel==vertex_label.end()) {
		cout << v_CUI << " does not exists in MSCONSO.RRF. Unexpected Error." << endl;
		exit(0);
	}
	else {
		pdata.path_str = v_CUI + ":" + iter_vLabel->second;
	}
	
	for(int i=0; i<path.size()-1; i++) {
		vertexPair e_tmp;
		int left=path[i];
		int right=path[i+1];
		e_tmp.first=left;
		e_tmp.second=right;
		pair<multimap<vertexPair,int>::iterator, multimap<vertexPair,int>::iterator> ret = edgeList.equal_range(e_tmp);
		multimap<vertexPair,int>::iterator edgeit = edgeList.find(e_tmp);
		
		if (distance(ret.first, ret.second) > 0) {
			multimap<vertexPair,int>::iterator edgeit;
			for (edgeit=ret.first; edgeit != ret.second; edgeit++)
			{
				if (forbiddenEdgeIDList.find(edgeit->second) == forbiddenEdgeIDList.end()) {
					string edge_label_tmp = edgeLabelList[edgeit->second];
					pdata.path_str += "\t" + edge_label_tmp;
					break;
				}
			}
			if (edgeit == ret.second) {
				cout << "Error in kDLS semantic restriction processing!" << endl;
			}
		}
		else {
			cout << "Unexpected Error: An edge does not exist in the edge ID file!" << endl;
			cout << left << "," << right << endl;
			exit(0);
		}
		
		v_CUI = VERTEX[right];
		iter_vLabel = vertex_label.find(v_CUI);
		
		if (iter_vLabel == vertex_label.end()) {
			cout << v_CUI << " does not exists in MSCONSO.RRF. Unexpected Error." << endl;
			exit(0);
		}
		else {
			pdata.path_str += "\t" + v_CUI + ":" + iter_vLabel->second;
		}
	}
	
	return pdata;
}

vector<vector<int> > process_all_shortest_path(const int &ss, const int &tt) {
	vector<vector<int> > allshortestpath;
	kDLS.find_allshortestpath(ss, tt, allshortestpath);
	return allshortestpath;
}

path_result process_single_job(const string &s, const string &t) {

	// convert strings to integers
	int ss = Util::strToNum(s);
	int tt = Util::strToNum(t);
	
	// calculate distance
	int dist = process_distance(ss, tt);
	
	// calculate all shortest paths
	vector<vector<int> > allshortestpath = process_all_shortest_path(ss, tt);
	
	// return values in a struct
	path_result p_res;
	p_res.distance = dist;
	p_res.allshortestpath = allshortestpath;
	
	cout << "process_single_job, distance: " << Util::numToStr(p_res.distance) << ", allshortestpath length : " << Util::numToStr(p_res.allshortestpath.size()) << endl;
	
	return p_res;
}

// return 0 if error occurs during reading/writing files, 1 if successful
int process_matrix_job(const string &special_key, const bool &email_exists) {
	// read input files
	string row_full_filename = "";
	string col_full_filename = "";


	if (email_exists) {
		row_full_filename = "./matrix_uploads/" + special_key + "_row.txt";
		col_full_filename = "./matrix_uploads/" +special_key + "_col.txt";
	}
	else {
		row_full_filename = "./matrix_temporary_uploads/" + special_key + "_row.txt";
		col_full_filename = "./matrix_temporary_uploads/" +special_key + "_col.txt";
	}

	ifstream in_row(row_full_filename.c_str());
	ifstream in_col(col_full_filename.c_str());
	
	if (!in_row || !in_col) {
		cout << "Error opening at least one file with key: " + special_key << endl;
		return 0;
	}
	
	// build row, col
	string sub, buf;
	vector<pair<string, rcInfo> > row;
	vector<pair<string, rcInfo> > col;
	
	int effective_row_size = 0;
	while (getline(in_row, buf)) {
		sub = buf.substr(0, buf.find("	"));
		
		// check for CR
		if (!sub.empty() && sub[sub.size() - 1] == '\r') {
			sub.erase(sub.size() - 1);
		}
		
		map<string,string>::iterator mit_label = vertex_label.find(sub);
		if (mit_label != vertex_label.end()){//if (find(row.begin(), row.end(), sub) == row.end()) { //(row.find(sub) == row.end()) {
			rcInfo rInfo_tmp;
			map<string, int>::iterator mit = vertexCUI_ID.find(sub);		
			if (mit == vertexCUI_ID.end()) {
				#ifdef _DEBUG_MODE
				cout << sub << " not found in semantic network." << endl;
				#endif
				rInfo_tmp.ID = -1;
			}
			else {
				rInfo_tmp.ID = mit->second;
				effective_row_size++;
			}
			rInfo_tmp.preferredName = mit_label->second;
			row.push_back(pair<string,rcInfo>(sub, rInfo_tmp));
		}
		#ifdef _DEBUG_MODE
		else{
			cout << sub << " not found in the MSCONSO.RRF." << endl;
		}
		#endif
	}
	in_row.close();
	
	int effective_col_size=0;
	while (getline(in_col, buf)) {
		sub = buf.substr(0, buf.find("	"));
		
		// check for CR
		if (!sub.empty() && sub[sub.size() - 1] == '\r') {
			sub.erase(sub.size() - 1);
		}
		
		map<string,string>::iterator mit_label = vertex_label.find(sub);
		if (mit_label != vertex_label.end()){//if (find(col.begin(), col.end(), sub) == col.end()) {  //(col.find(sub) == col.end()) {
			rcInfo cInfo_tmp;
			map<string, int>::iterator mit = vertexCUI_ID.find(sub);
			if (mit == vertexCUI_ID.end()) {
				#ifdef _DEBUG_MODE
				cout << sub << " not found in semantic network." << endl;
				#endif
				cInfo_tmp.ID = -1;
			}
			else {
				cInfo_tmp.ID = mit->second;
				effective_col_size++;
			}
			cInfo_tmp.preferredName = mit_label->second;
			col.push_back(pair<string,rcInfo>(sub,cInfo_tmp));
		}
		#ifdef _DEBUG_MODE
		else{
			cout << sub << " not found in the MSCONSO.RRF." << endl;
		}
		#endif
	}
	in_col.close();
	
	int result = build_knowledge_matrices(row, col, special_key, email_exists);
	
	return result;
}

void *single_worker_thread(void *arg) {

	long tid = pthread_self();
	job_data *temp = (job_data *)arg;
	
	// make local copy
	int job_id = temp->job_id;
	int slot_id = temp->slot_id;
	
	// iinit database connection
	DBManager dbm = DBManager("localhost", "user_name", "password", "db_name", 0);
	
	Singledata sd = dbm.get_single_job(job_id);
	
	string s_jid = Util::numToStr(sd.get_job_id());
	string s_tid = Util::numToStr(tid);
	string s_sid = Util::numToStr(slot_id);
	
	// acquire logger mutex
	pthread_mutex_lock(&mutex);
	logger.write("Thread id: " + s_tid + "\tusing slot: " + s_sid);
	// release logger mutex
	pthread_mutex_unlock(&mutex);
	
	// add job data
	dbm.set_slot(slot_id, tid, sd.get_job_id(), 0);
	
	// handle job
	path_result p_res = process_single_job(sd. get_start_id(), sd.get_end_id());
	
	string p_result_build_str = "";
	// write paths to large string, separated by '$', limit string size to 2500 bytes
	if (p_res.allshortestpath.size() != 0) {
		string temp_str = "";
		std::size_t	i, max;
		int limit = 60000;
		vector<path_data> pdata;
	
		for (vector<vector<int> >::iterator iter_allshortestpath = p_res.allshortestpath.begin(); iter_allshortestpath != p_res.allshortestpath.end(); iter_allshortestpath++) {
			pdata.push_back(pathdecoder(*iter_allshortestpath));
		}
		
		for (i = 0, max = pdata.size(); i != max; ++i) {
			string new_str = temp_str + pdata[i].path_str + "$";
			if (new_str.size() > limit) {
				break;
			}
			temp_str = new_str;
			//if (i >= limit) {
			//	break;
			//}
			//temp_str += pdata[i].path_str + "$";
		}
		// remove last '$'
		p_result_build_str = temp_str.substr(0, temp_str.size()-1);
	}
	
	// write result to db
	dbm.update_single_completed(sd.get_job_id(), p_res.distance, p_result_build_str);
	
	// acquire logger mutex
	pthread_mutex_lock(&mutex);
	logger.write("Thread id: " + s_tid + "\tcompleted job id: " + s_jid);
	// release logger mutex
	pthread_mutex_unlock(&mutex);
	
	// open up slot
	dbm.release_slot(slot_id);
	
	// clean up and close
	dbm.close();
	pthread_exit(NULL);
}

void *matrix_worker_thread(void *arg) {

	long tid = pthread_self();
	job_data *temp = (job_data *)arg;
	
	// make local copy
	int job_id = temp->job_id;
	int slot_id = temp->slot_id;
	
	cout << "local copy is : " << Util::numToStr(job_id) << ", " << Util::numToStr(slot_id) << endl;
	
	// iinit database connection
	DBManager dbm = DBManager("localhost", "user_name", "password", "db_name", 0);
	
	// make local copy
	Matrixdata md = dbm.get_matrix_job(job_id);
	cout << "matrix data...:" << md.to_string() << endl;
	
	cout << "Processing....matrix job: " << endl;
	cout << md.to_string() << endl;

	// check if job has email attached (used for emailing, also to determine if job is in temporary_uploads)
	bool email_exists = dbm.user_email_exists(md.get_job_id());
	
	string s_jid = Util::numToStr(md.get_job_id());
	string s_tid = Util::numToStr(tid);
	string s_sid = Util::numToStr(slot_id);
	
	// acquire logger mutex
	pthread_mutex_lock(&mutex);
	logger.write("Thread id: " + s_tid + "\tusing slot: " + s_sid);
	// release logger mutex
	pthread_mutex_unlock(&mutex);
	
	// add job data
	dbm.set_slot(slot_id, tid, md.get_job_id(), 1);
	
	// handle job
	dbm.close();

	// check if it will be a long job (file upload)...
	if (email_exists) {
		// set affinity to designated slot/core
		int set_thread = set_thread_to_core(tid, slot_id);
		cout << "set_thread returns: " << Util::numToStr(set_thread) << endl;

		// set priority of thread to batch
		sched_get_priority_min(SCHED_BATCH);
		pthread_setschedparam(tid, SCHED_BATCH, &sched_param);

	}

	int success = process_matrix_job(md.get_special_key(), email_exists);
	
	dbm = DBManager("localhost", "user_name", "password", "db_name", 0);
	if (success) {
		// write result to db
		dbm.update_matrix_completed(md.get_job_id());

		// send email to user if email exists (if email exists, job was uploaded)
		if (email_exists) {
			cout << "Sending email...." << endl;
			string msg = string("/usr/bin/python scripts/emailer.py ") + Util::numToStr(job_id);
			int r = system(msg.c_str());
		}
	}
	else {
		// error with file i/o
		dbm.update_matrix_error(md.get_job_id());
	}
	
	// acquire logger mutex
	pthread_mutex_lock(&mutex);
	logger.write("Thread id: " + s_tid + "\tcompleted job id: " + s_jid);
	// release logger mutex
	pthread_mutex_unlock(&mutex);
	
	// open up slot
	dbm.release_slot(slot_id);
	
	// clean up and close
	dbm.close();
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	logger.init("log.out");
	pthread_mutex_init(&mutex, NULL);

	char* filename="graph_Forbid.gra";
	length_limit = 6;
	EXPBASE = 7;
	char* sinksourcefile="sinksource_Forbid_file.txt";
	char* vertexCUIfile = "vertexCUI.txt";
	char* vertexLabelfile = "VertexLabelFile.txt";
	char* edgeIDFile = "EdgeIDFile.txt";
	char* edgeLabelFile = "EdgeLabelFile.txt";
	char* eforbiddenFile = "eForbidden.txt";
	
	//read the label file
	char* in_label="Lin_Forbid_Label_6";
	char* out_label="Lout_Forbid_Label_6";
	char* in_out_same_label="Linoutsame_Forbid_Label_6";
	char* in_out_diff_label="Linoutdiff_Forbid_Label_6";
	char* special_label="Lspecial_Forbid_Label_6";
	char* vertex_ranking = "VertexRanking.txt";
	

	ifstream infile(filename);
	ifstream in_sinksource(sinksourcefile);

	// added for all shortest paths
	ifstream in_vCUI(vertexCUIfile);
	ifstream in_vLabel(vertexLabelfile);
	ifstream in_edgeID(edgeIDFile);
	ifstream in_edgeLabel(edgeLabelFile);	
	ifstream in_e_forbidden(eforbiddenFile);  // in 4, don't use
	ifstream inrankfile(vertex_ranking);
	
	if (!infile || !in_vCUI || !in_vLabel || !in_edgeID || !in_edgeLabel || !in_sinksource || !in_e_forbidden || !inrankfile) {
		cout << "Error in opening at least one input file. Exit." << endl;
		exit(0);
	}
	
	// load graph
	g.readGraph(infile, inrankfile);
	logger.write("Number of vertices in the graph:" + Util::numToStr(g.num_vertices()));
	infile.close();
	inrankfile.close();
	
	// load labels
	kDLS.init(in_label, out_label, in_out_same_label, in_out_diff_label, special_label);
	
	//read the vertex CUI file into a vector
	string buf;
	int n = 0;
	while (getline(in_vCUI, buf)) {
		VERTEX.push_back(buf);
		vertexCUI_ID.insert(pair<string,int>(buf,n));  // added for matrix: read the vertex CUI file, each concept map to a integer
		n++;
	}
	in_vCUI.close();
	//******************************************************************
	
	//read the vertex label file, each vertex map to a meanning
	while (getline(in_vLabel, buf)) {
		string sub=buf.substr(0, buf.find("	"));
		buf.erase(0, buf.find("	")+1);
		vertex_label.insert(pair<string,string>(sub,buf));
	}
	in_vLabel.close();
	//***************************************************************
	
	//read the edge ID file
	n = 0;
	while (getline(in_edgeID, buf)) {//first-->second
		vertexPair e;
		istringstream(buf.substr(0, buf.find(" "))) >> e.second;
		buf.erase(0, buf.find(" ")+1);
		istringstream(buf.substr(0, buf.find(" "))) >> e.first;
		buf.erase(0, buf.find(" ")+1);
		int edgeLabelID;
		istringstream(buf) >> edgeLabelID;
		edgeList.insert(pair<vertexPair,int>(e,edgeLabelID));
		n++;
	}
	in_edgeID.close();
	cout << "Total number of edges loaded from edgeIDFile: " << n << endl;
	//**************************************************
	
	//read the edge label file
	n = 0;
	while (getline(in_edgeLabel, buf)) {
		edgeLabelList.push_back(buf);
		n++;
	}
	in_edgeLabel.close();
	cout << "Total number of edge labels loaded from edgeLabelFile: " << n << endl;
	//*********************************************

	//read forbidden edge file
	vector<string> forbiddenEdgeList;
	while (getline(in_e_forbidden, buf)) {
		forbiddenEdgeList.push_back(buf);
	}		
	
	for (vector<string>::iterator iter_forbid=forbiddenEdgeList.begin(); iter_forbid!=forbiddenEdgeList.end(); iter_forbid++) {
		vector<string>::iterator forbid_edge_index=find(edgeLabelList.begin(),edgeLabelList.end(),*iter_forbid);
		if (forbid_edge_index!=edgeLabelList.end()) {
			forbiddenEdgeIDList.insert(distance(edgeLabelList.begin(),forbid_edge_index));
		}
		else {
			cout << "Forbidden Edge " << *iter_forbid << " not found in edge label file" << endl;
		}			
	}
	in_e_forbidden.close();	
	//*********************************************
	
	//read the sink source file, which contain the nodes with in_degree==0 and out_degree>=10 or out_degree==0 and in_degree>=10
	while (getline(in_sinksource, buf)) {
		int k;
		istringstream(buf) >> k;
		sinksourcenodes.insert(k);
	}
	in_sinksource.close();
	cout << "Number of sinks and sources:" << sinksourcenodes.size() << endl;	
	
	//****************....finished preloading....**************************
	logger.write("Preloading finished, waiting for new jobs...");
	
	int rc;
	std::size_t	i, max;
	vector<int> slot_ids;
	// init database connection
	DBManager dbm = DBManager("localhost", "user_name", "password", "db_name", 1);
	
	while (true) {
		vector<job_data> single_jobs;
	
		/*******     Check for single jobs first     *******/
		vector<int> single_job_ids = dbm.check_single_jobs();
		
		// if new jobs exists
		if (single_job_ids.size() > 0) {
			// get available slot ids
			slot_ids = dbm.get_free_slot_ids(single_job_ids.size());
			
			// check if any jobs need to be rejected (ie not enough available slots)
			if (slot_ids.size() < single_job_ids.size()) {
				for (i = slot_ids.size(), max = single_job_ids.size(); i != max; ++i) {
					dbm.update_rejected(single_job_ids[i], 0);
				}
				
				// remove rejected jobs from stored_data
				single_job_ids.erase(single_job_ids.begin() + slot_ids.size(), single_job_ids.end());
			}
			
			// update jobs as processing
			for (i = 0, max = single_job_ids.size(); i != max; ++i) {
				dbm.update_processing(single_job_ids[i], 0);
				job_data jd;
				jd.job_id = single_job_ids[i];
				jd.slot_id = slot_ids[i];
				cout << "added job with id: " << Util::numToStr(jd.job_id) << ", and slot id: " << jd.slot_id << endl;
				single_jobs.push_back(jd);
			}
			
			// spawn enough threads to handle jobs
			for (i = 0, max = single_jobs.size(); i != max; ++i) {
				pthread_t temp;
				rc = pthread_create(&temp, NULL, single_worker_thread, (void *) &single_jobs[i]);
				if (rc) {
					// acquire logger mutex
					pthread_mutex_lock(&mutex);
					logger.write("ERROR; return code from pthread_create() is: " + Util::numToStr(rc));
					// release job queue mutex
					pthread_mutex_unlock (&mutex);
					exit(-1);
				}
			}
		}
		
		vector<job_data> matrix_jobs;
		
		/*******     Check for matrix jobs    *******/
		vector<int> matrix_job_ids = dbm.check_matrix_jobs();
		
		// if new jobs exists
		if (matrix_job_ids.size() > 0) {
		
			// get available slot ids
			slot_ids = dbm.get_free_slot_ids(matrix_job_ids.size());

			// acquire logger mutex
			pthread_mutex_lock(&mutex);
			logger.write("number of available slots is::: " + Util::numToStr(slot_ids.size()));
			// release job queue mutex
			pthread_mutex_unlock (&mutex);
			
			//check if any jobs need to be rejected (ie not enough available slots)
			if (slot_ids.size() < matrix_job_ids.size()) {
				for (i = slot_ids.size(), max = matrix_job_ids.size(); i != max; ++i) {
					dbm.update_rejected(matrix_job_ids[i], 1);
				}
				
				//remove rejected jobs from stored_data
				matrix_job_ids.erase(matrix_job_ids.begin() + slot_ids.size(), matrix_job_ids.end());
			}
			
			// update jobs as processing, assign slot id, add lengthlimit
			for (i = 0, max = matrix_job_ids.size(); i != max; ++i) {
				dbm.update_processing(matrix_job_ids[i], 1);
				job_data jd;
				jd.job_id = matrix_job_ids[i];
				jd.slot_id = slot_ids[i];
				cout << "job is " << Util::numToStr(jd.job_id) << ", " << Util::numToStr(jd.slot_id) << endl;
				matrix_jobs.push_back(jd);
			}
			
			// spawn enough threads to handle jobs
			for (i = 0, max = matrix_jobs.size(); i != max; ++i) {
				pthread_t temp;
				cout << "create thread with job: " << Util::numToStr(matrix_jobs[i].job_id) << ", " << Util::numToStr(matrix_jobs[i].slot_id) << endl;
				rc = pthread_create(&temp, NULL, matrix_worker_thread, (void *) &matrix_jobs[i]);
				if (rc) {
					// acquire logger mutex
					pthread_mutex_lock(&mutex);
					logger.write("ERROR; return code from pthread_create() is: " + Util::numToStr(rc));
					// release job queue mutex
					pthread_mutex_unlock (&mutex);
					exit(-1);
				}
			}
		}
		
		sleep(2);
		
		// just making sure...
		slot_ids.clear();
		single_job_ids.clear();
		matrix_job_ids.clear();
		single_jobs.clear();
		matrix_jobs.clear();
	}
	
	// clean up and exit
	dbm.close();
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
	
	return 0;
}


