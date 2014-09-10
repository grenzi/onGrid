
#include "LDPWrapper.h"


LDPWrapper::LDPWrapper() : m_numResults(1) {
}

LDPWrapper::~LDPWrapper() {
}

bool LDPWrapper::init(const float& threshold1, const float& threshold2, const bool& case_sensitive, const std::string& filename) {
	m_dict.init(threshold1, threshold2, case_sensitive);
	if(!m_dict.read(filename.c_str())) {
		return false;
	}
	return true;
}

void LDPWrapper::match_worker(job_data& x) {

	time_t before = time(0);

	std::string result;
	int matchNum;
	
	if (x.type == 1) {
		matchNum = 1;
	}
	else {  // assume 2, 3, 4
		matchNum = 10;
	}	
	
	result = m_dict.match(x.query, matchNum);
	
	/*
	time_t after = time(0);
	boost::mutex::scoped_lock lock(m_mutex);
	tm *beforelocal = localtime(&before);
	std::cout << "before: " << asctime(beforelocal);
	tm *afterlocal = localtime(&after);
	std::cout << "after: " << asctime(afterlocal);
	std::cout << x.query + " : " + result << std::endl;
	*/
	x.result = result;
}

std::vector<job_data> LDPWrapper::get_matches(const std::vector<std::string>& queries, const int& type) {
	
	boost::thread_group g;
	std::vector<job_data> results;

	for (int i = 0; i < queries.size(); i++) {
		job_data x(queries[i], type);
		results.push_back(x);
	}
	
	for (int i = 0; i < queries.size(); i++) {
		g.create_thread(boost::bind(&LDPWrapper::match_worker, this, boost::ref(results[i])));
	}

	// wait for all results
	g.join_all();

	return results;
}
