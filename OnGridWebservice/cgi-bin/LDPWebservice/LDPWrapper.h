#ifndef _LDPWrapper_H
#define _LDPWrapper_H

#include <boost/thread.hpp>
#include <vector>
#include <string>
#include <ctime>
#include "Dictionary.h"

struct job_data {
	std::string query;
	std::string result;
	int type;
	job_data(std::string s, int t) : result("") {
		query = s;
		type = t;
	}
};

class LDPWrapper {

	private:
		Dictionary m_dict;
		int m_numResults;
		boost::mutex m_mutex;
		void match_worker(job_data& x);

	public:

		LDPWrapper();
		~LDPWrapper();
		bool init(const float& threshold1, const float& threshold2, const bool& case_sensitive, const std::string& filename);
		std::vector<job_data> get_matches(const std::vector<std::string>& queries, const int& type);
};

#endif

