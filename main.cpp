#include <iostream>
#include <fstream>

using namespace std;

uint compair(std::istream& f1, std::istream& f2) {

}

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

	ifstream file1();
	ifstream file2();

	file1.exceptions(ifstream::failbit | ifstream::badbit);
	file2.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		file1.open(fn1, ios::binary);
		file2.open(fn2, ios::binary);

		file1.seekg(0, file1.end);
		file2.seekg(0, file2.end);

		if(file1.tellg() != file2.tellg()) {
			cerr << "Rózna długość strumieni!" << endl;
			return -3;
		}

		uint size = file1.tellg() * 8; // na każdy bajt przypada 8 bitów
		uint bad;

		file1.seekg(0, file1.beg);
		file2.seekg(0, file2.beg);

		compair(file1, file2);

		file2.close();
		file1.close();
	}
	catch(ifstream::failure error) {
		cerr << "Błąd otwarcia pliku!" << endl;
	}
	
	return 0;
}