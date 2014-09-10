#ifndef SINGLEDATA_H
#define SINGLEDATA_H

#include "Data.h"

class Singledata : public Data {
	private:
		std::string m_start_id;
		std::string m_end_id;
	
	public:
		Singledata();
		Singledata(const int &job_id, const std::string &start_id, const std::string &end_id);
		Singledata(const int &job_id, const int &slot_id, const std::string &start_id, const std::string &end_id);
		~Singledata();
		std::string get_start_id();
		std::string get_end_id();
		void set_start_id(const std::string &start_id);
		void set_end_id(const std::string &end_id);
		std::string to_string();
};

#endif
