#include "Matrixdata.h"

Matrixdata::Matrixdata() : Data(1), m_row_filename(""), m_col_filename(""), m_special_key("") {
	// nothing
}

Matrixdata::Matrixdata(const int &job_id, const std::string &special_key) : Data(job_id, 1), m_special_key(special_key) {
	// nothing
}

Matrixdata::Matrixdata(const int &job_id, const int &slot_id, const std::string &special_key) : Data(job_id, slot_id, 1), m_special_key(special_key) {
	// nothing
}

Matrixdata::~Matrixdata() {

}

std::string Matrixdata::get_special_key() {
	return m_special_key;
}

void Matrixdata::set_special_key(const std::string &special_key) {
	m_special_key = special_key;
}

std::string Matrixdata::to_string() {
	return Data::to_string() + ", key: " + m_special_key;
}
