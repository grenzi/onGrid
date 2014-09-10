#include "Singledata.h"
#include "Util.h"

Singledata::Singledata(): Data(0), m_start_id(""), m_end_id("") {
	// nothing
}

Singledata::Singledata(const int &job_id, const std::string &start_id, const std::string &end_id) : Data(job_id, 0), m_start_id(start_id), m_end_id(end_id) {
	// nothing
}

Singledata::Singledata(const int &job_id, const int &slot_id, const std::string &start_id, const std::string &end_id) : Data(job_id, slot_id, 0), m_start_id(start_id), m_end_id(end_id) {
	// nothing
}

Singledata::~Singledata() {

}

std::string Singledata::get_start_id() {
	return m_start_id;
}

std::string Singledata::get_end_id() {
	return m_end_id;
}

void Singledata::set_start_id(const std::string &start_id) {
	m_start_id = start_id;
}

void Singledata::set_end_id(const std::string &end_id) {
	m_end_id = end_id;
}

std::string Singledata::to_string() {
	return Data::to_string() + ", start_id: " + m_start_id + ", end_id: " + m_end_id;
}
