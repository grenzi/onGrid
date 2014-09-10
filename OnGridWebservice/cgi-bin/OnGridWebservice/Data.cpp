#include "Data.h"
#include "Util.h"

Data::Data() : m_job_id(0), m_slot_id(0), m_type(0) {
	// nothing
}

Data::Data(const int &type) : m_type(type) {
	// nothing
}

Data::Data(const int &job_id, const int &type) : m_job_id(job_id), m_type(type) {
	// nothing
} 

Data::Data(const int &job_id, const int &slot_id, const int &type) : m_job_id(job_id), m_slot_id(slot_id), m_type(type) {
	// nothing
}

Data::~Data() {

}

int Data::get_job_id() {
	return m_job_id;
}

int Data::get_slot_id() {
	return m_slot_id;
}

int Data::get_type() {
	return m_type;
}

void Data::set_job_id(const int &job_id) {
	m_job_id = job_id;
}

void Data::set_slot_id(const int &slot_id) {
	m_slot_id = slot_id;
}

void Data::set_type(const int &type) {
	m_type = type;
}

std::string Data::to_string() {
	return "job_id: " + Util::numToStr(m_job_id);
}
