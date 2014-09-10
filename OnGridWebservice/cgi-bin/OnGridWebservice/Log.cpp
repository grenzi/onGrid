#include <string>
#include "Log.h"

Log::Log() {
}

Log::Log(const std::string &filename) {
	init(filename);
}

Log::~Log() {
	m_stream.close();
}

void Log::init(const std::string &filename) {
	std::string path = filename;
	m_stream.open(path.c_str(), std::ios::out | std::ios::app);
}

void Log::write(const std::string &line) {
	time_t now = time(0);
	tm *localtm = localtime(&now);
	m_stream << std::endl << asctime(localtm);
	m_stream << line << std::endl;
}
