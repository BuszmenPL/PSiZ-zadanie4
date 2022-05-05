#include <iostream>
#include <fstream>
#include <chrono>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace std;
using namespace chrono;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint compair(std::istream&, std::istream&);
uint count(unsigned char, unsigned char);
void save(uint, uint, std::chrono::milliseconds);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, const char* argv[])
{
	if(argc != 3) {
		cerr << "Nieprawidłowa ilość argumentów!" << endl;
		return -1;
	}

	string fn1(argv[1]);
	string fn2(argv[2]);

	if(fn1 == fn2) {
		cerr << "Pliki muszą byc różne!" << endl;
		return -2;
	}

	ifstream file1;
	ifstream file2;

	file1.exceptions(ifstream::failbit | ifstream::badbit);
	file2.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		file1.open(fn1, ios::binary);
		file2.open(fn2, ios::binary);

		file1.seekg(0, ifstream::end);
		file2.seekg(0, ifstream::end);

		if(file1.tellg() != file2.tellg()) { // porówanie długości plików
			cerr << "Różna długość strumieni!" << endl;
			return -3;
		}

		uint size = file1.tellg() * 8; // na każdy bajt przypada 8 bitów
		uint bad;

		file1.seekg(0, ifstream::beg);
		file2.seekg(0, ifstream::beg);

		steady_clock::time_point start = steady_clock::now();

		bad = compair(file1, file2);

		steady_clock::time_point stop = steady_clock::now();

		file2.close();
		file1.close();

		save(size, bad, duration_cast<milliseconds>(stop - start));
	}
	catch(ifstream::failure error) {
		cerr << "Błąd odczytu pliku!" << endl;
	}
	
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint compair(std::istream& src, std::istream& dst) {
	char s, d;
	uint n{};

	try {
		while(src.get(s) && dst.get(d))
			if(s != d)
				n += count(s, d);
	}
	catch(istream::failure&) { // Ustawienie eof może powodować błąd!
		if(!(src.eof() || dst.eof()))
			throw;
	}

	return n;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint count(unsigned char s, unsigned char d) {
	uint n{};

	s = s ^ d;

	while(s) {
		if(s & 1)
			++n;
		s >>= 1;
	}

	return n;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void save(uint all, uint bad, std::chrono::milliseconds time) {
	cout << "Ilośc porównanych bitów: " << all << endl << "Ilość różnych bitów: " << bad << endl;
	cout << "BER: " << bad << '/' << all << endl << "Czas obliczeń: " << time.count() << "ms" << endl;

	ofstream log;
	log.exceptions(ofstream::failbit | ofstream::badbit);

	try {
		log.open("log.txt", ios::app);

		log << time.count() << ' ' << all << ' ' << bad << ' ' << bad << '/' << all << endl;

		log.close();
	}
	catch(ofstream::failure error) {
		cerr << "Błąd zapisu do pliku log.txt" << endl;
	}
}