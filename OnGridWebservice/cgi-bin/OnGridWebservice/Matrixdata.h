#ifndef MATRIXDATA_H
#define MATRIXDATA_H

#include "Data.h"

class Matrixdata : public Data {
	private:
		std::string m_row_filename;
		std::string m_col_filename;
		std::string m_special_key;
	
	public:
		Matrixdata();
		Matrixdata(const int &job_id, const std::string &special_key);
		Matrixdata(const int &job_id, const int &slot_id, const std::string &special_key);
		~Matrixdata();
		std::string get_special_key();
		void set_special_key(const std::string &special_key);
		std::string to_string();
};

#endif
