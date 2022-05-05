#include "ber.h"
#include <fstream>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace std;
using namespace chrono;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BER::BER(std::istream& f, std::istream& s)
	:_size{}, _bad{}, _time{} {

	steady_clock::time_point start = steady_clock::now();

	compair(f, s);

	steady_clock::time_point stop = steady_clock::now();

	_time = stop - start;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double BER::time_milliseconds() {
	return static_cast<double>(this->time().count()) / 1000000.0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double BER::value() const {
	return static_cast<double>(this->bad()) / static_cast<double>(this->size());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool BER::save(std::string f) {
	ofstream log;
	log.exceptions(ofstream::failbit | ofstream::badbit);

	try {
		log.open(f, ios::app);

		log << this->time_milliseconds() << "ms " << this->size() << ' ';
		log << this->bad() << ' ' << this->value() << endl;

		log.close();

		return true;
	}
	catch(ofstream::failure error) {
		return false;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void BER::compair(std::istream& src, std::istream& dst) {
	char s, d;
	uint32_t n{};
	uint32_t sz{};

	try {
		while(src.get(s) && dst.get(d)){
			if(s != d)
				n += this->count(s, d);

			++sz;
		}
	}
	catch(istream::failure&) { // Ustawienie eof może powodować błąd!
		if(!(src.eof() || dst.eof()))
			throw;
	}

	this->_size = sz * 8; // na każdy bajt przypada osiem bitów
	this->_bad = n;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint32_t BER::count(unsigned char s, unsigned char d) {
	uint32_t n{};

	s = s ^ d;

	while(s) {
		if(s & 1)
			++n;
		s >>= 1;
	}

	return n;
}