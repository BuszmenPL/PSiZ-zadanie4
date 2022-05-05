#include <iostream>
#include <fstream>
#include "ber.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace std;
using namespace chrono;
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

	// w razie błędu rzuć wyjątek
	file1.exceptions(ifstream::failbit | ifstream::badbit);
	file2.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		file1.open(fn1, ios::binary);
		file2.open(fn2, ios::binary);

		// oblicza Bit Error Rate
		BER ber(file1, file2);
		// Nie zamyka pliku, destruktor to zrobi

		cout << "Ilośc porównanych bitów: " << ber.size() << endl;
		cout << "Ilość różnych bitów: " << ber.bad() << endl;
		cout << "BER: " << ber.bad() << '/' << ber.size() << endl;
		cout << "Czas obliczeń: " << duration_cast<milliseconds>(ber.time()).count() << "ms" << endl;

		// zapis informacji w pliku log.txt
		if(!ber.save("log.txt"))
			cerr << "Błąd zapisu!" << endl;
	}
	catch(ifstream::failure error) {
		cerr << "Błąd odczytu pliku!" << endl;
	}
	
	return 0;
}