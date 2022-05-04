#include <iostream>
#include <fstream>

using namespace std;

uint compair(std::istream&, std::istream&);
uint count(char, char);

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

		file1.seekg(0, file1.end);
		file2.seekg(0, file2.end);

		if(file1.tellg() != file2.tellg()) { // porówanie długości plików
			cerr << "Rózna długość strumieni!" << endl;
			return -3;
		}

		uint size = file1.tellg() * 8; // na każdy bajt przypada 8 bitów
		uint bad;

		file1.seekg(0, file1.beg);
		file2.seekg(0, file2.beg);

		bad = compair(file1, file2);

		cout << "Size file: " << size << ", Bad: " << bad << endl;

		file2.close();
		file1.close();
	}
	catch(ifstream::failure error) {
		cerr << "Błąd otwarcia pliku! " << error.what() << endl;
	}
	
	return 0;
}

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

uint count(char s, char d) {
	uint n{};

	s = s ^ d;

	while(s) {
		if(s & 1)
			++n;
		s >> 1;
	}

	return n;
}