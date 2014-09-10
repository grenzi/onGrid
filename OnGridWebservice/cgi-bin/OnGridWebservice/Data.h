#ifndef DATA_H
#define DATA_H

#include <string>

class Data{
	protected:
		int m_job_id,  m_slot_id, m_type;  // 0 is single, 1 is matrix
	
	public:
		Data();  // default constructor
		Data(const int &type);
		Data(const int &job_id, const int &type);
		Data(const int &job_id, const int &slot_id, const int &type);
		~Data();
		int get_job_id();
		int get_slot_id();
		int get_type();
		void set_job_id(const int &job_id);
		void set_slot_id(const int &slot_id);
		void set_type(const int &m_type);
		std::string to_string();
};

#endif
