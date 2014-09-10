#include "DBJob.h"

DBJob::DBJob() {
}

DBJob::DBJob(const std::string& terms, const int& type) : m_terms(terms), m_type(type) {
}

DBJob::~DBJob() {
}

std::string DBJob::getTerms() {
	return m_terms;
}

int DBJob::getType() {
	return m_type;
}
