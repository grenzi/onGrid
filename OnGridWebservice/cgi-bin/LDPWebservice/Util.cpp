
#include "Util.h"

namespace Util {

	std::string numToStr(const int &num) {
		std::stringstream sstm;
		sstm << num;
		return sstm.str();
	}

	std::string numToStr(const long &num) {
		std::stringstream sstm;
		sstm << num;
		return sstm.str();
	}

	std::string numToStr(const double &num) {
		std::stringstream sstm;
		sstm << num;
		return sstm.str();
	}
	
	std::string numToStr(const size_t &num) {
		std::stringstream sstm;
		sstm << num;
		return sstm.str();
	}
	
	int strToNum(const std::string &str) {
		int val;
		std::stringstream sstm(str);
		if (!(sstm >> val)) {
			val = 0;
		}
		return val;
	}
}
