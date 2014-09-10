
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "DBManager.h"
#include "Log.h"
#include "Util.h"
#include "LDPWrapper.h"
#include "DBJob.h"
using namespace std;

// global variables
Log logger;
LDPWrapper ldp;

vector<string> split(const string &s, const char& delim) {
	vector<string> tokens;
    stringstream ss(s);
    string tok;
    while (getline(ss, tok, delim)) {
        tokens.push_back(tok);
    }
    return tokens;
}

string build_result_str(const vector<job_data>& matches) {
	string result_str = "";
	for (size_t i = 0; i < matches.size(); i++) {
		if (i != matches.size() - 1)
			result_str += matches[i].result + "$";
		else
			result_str += matches[i].result;
	}
	return result_str;
}

string process_job(DBJob& job) {

	std::vector<string> queries = split(job.getTerms(), '$');
	std::vector<job_data> matches = ldp.get_matches(queries, job.getType());
	
	// build results as $-separated string
	string results_str = build_result_str(matches);

	return results_str;
}

void worker_thread(const int job_id) {
	
	// init database connection
	DBManager dbm = DBManager("localhost", "user_name", "password", "db_name", 0);
	
	DBJob job = dbm.get_job(job_id);
	
	cout << "job terms: " << job.getTerms() << ", job type: " << job.getType() << endl;
	
	vector<string> queries = split(job.getTerms(), '$');
	vector<job_data> matches = ldp.get_matches(queries, job.getType());
	
	if (job.getType() == 1) {
		// build results as $-separated string
		string results_str = build_result_str(matches);
		cout << "result string: " << results_str << endl;
		dbm.update_completed_type134(job_id, results_str, job.getType());
	}
	else if (job.getType() == 2) {  // return 10 options for start and end terms
		dbm.update_completed_type2(job_id, matches[0].result, matches[1].result);
	}
	else if (job.getType() == 3 || job.getType() == 4) {
		dbm.update_completed_type134(job_id, matches[0].result, job.getType());
	}
	
	// clean up and close
	dbm.close();
}

int main(int argc, char* argv[]) {
	logger.init("log.out");

	// arguments are threshold1, threshold2, case_sensitive, filename
	if (!ldp.init(0.85f, 0.35f, false, "MRCONSO.RRF")) {
		logger.write("Error loading dictionary.");
		exit(1);
	}
	
	//****************....finished preloading....**************************
	logger.write("Preloading finished, waiting for new jobs.");
	
	// init database connection
	DBManager dbm = DBManager("localhost", "user_name", "password", "db_name", 1);
	while (true) {
		vector<int> job_ids = dbm.check_jobs();
		
		// if new jobs exists
		if (job_ids.size() > 0) {
			
			// update jobs as processing
			for (vector<int>::size_type i = 0; i != job_ids.size(); i++) {
				dbm.update_processing(job_ids[i]);
			}
			
			// spawn enough threads to handle jobs
			for (vector<int>::size_type i = 0; i != job_ids.size(); i++) {
				boost::thread t(&worker_thread, job_ids[i]);
			}
		}
		
		sleep(2);
		job_ids.clear();
	}
	
	// clean up and exit
	dbm.close();
	
	return 0;
}


