
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>

namespace Util {
	std::string numToStr(const int &num);
	std::string numToStr(const long &num);
	std::string numToStr(const double &num);
	std::string numToStr(const size_t &num);
	int strToNum(const std::string &str);
}

#endif
