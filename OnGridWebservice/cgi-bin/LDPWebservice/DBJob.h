#ifndef DBJOB_H
#define DBJOB_H

#include <string>
#include <iostream>

class DBJob {
	private:
		std::string m_terms;
		int m_type;
	
	public:
		DBJob();
		DBJob(const std::string& terms, const int& type);
		~DBJob();
		std::string getTerms();
		int getType();
};

#endif
