#ifndef BER_H
#define BER_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <istream>
#include <chrono>
#include <string>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class BER
{
	public:
		using time_type = std::chrono::nanoseconds;

		BER(std::istream& f, std::istream& s);

		uint32_t size() const
			{ return this->_size; }

		uint32_t bad() const
			{ return this->_bad; }

		time_type time()
			{ return this->_time; }

		bool save(std::string f);

	private:

		uint32_t _size;
		uint32_t _bad;
		time_type _time;

		void compair(std::istream&, std::istream&);
		uint32_t count(unsigned char, unsigned char);
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif