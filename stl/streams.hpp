#pragma once

#include <ostream>
#include <istream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace filesystem;

void fix_out(path p)
{
	ifstream f{p};
	if(f)
	{
		ostream out{cout.rdbuf()};
		//to oznacza ,ze out oraz cout maja wspoldzelic bufor
		string s;
		while(getline(f,s))
		{
			out << s;
		}
	}
}

void mixed_buffers(path p)
{
	filebuf buffer;
	//zauwaz ,ze to nie jest streambuf
	ostream out{&buffer};
	istream in{&buffer};
	buffer.open(p,ios::in | ios::out | ios::app);
	for(size_t i{0}; i < 4; ++i)
	{
		out << i << ". wiersz" << endl;

		in.seekg(0u);

		char c;

		while (in.get(c))
			cout.put(c);

		cout << endl;
		in.clear();// pamietaj ,jesli masz jeden strumien i dojdziesz nim do konca pliku
		//to jesli zechcesz uzyc ponownie tego samego strumienia musisz wywolac  funkcje clear()
		//w celu usuniecia bitu eofbit
	}
}

void streamTests(path p)
{
	cout << "\nSTREAMS TESTING" << endl;
	string str {"abc 123"};
	cout << str << endl;
	stringstream ss{str};
	cout << "stream content : " << ss.str() << endl;
	ss.seekp(ss.str().length()); // kurewko kosztowne ,szczegolnie dla duzych lancuchow , musi istniec inne rozwiazanie
	//czyli najpierw ladujesz bufor ,a potem zwracasz stringa , bez kombinowania
	ss << "XXX";
	ss.flush();//nie wiem ,czy potrzebne
	cout << "stream content : " << ss.str() << endl;
	cout << str << endl;
	ss.~basic_stringstream();
	cout << "\nLOW LEVEL STREAM OPERATIONS" << endl;
	filebuf buffer;
	istream in{&buffer};
	ostream out{&buffer};
	buffer.open(p,ios::app | ios::out | ios::in);
	buffer.sputc('%');
	buffer.sputn("+=",2);//drugi argument okresla to ile znakow z lancucha  char const* ma zostac wyslane
	buffer.sputn("+=",1);//do bufora strumienia
	char a = '2';
	a = buffer.sgetc();

	cout << a << endl;
	a = buffer.sgetc();
	cout << a << endl;

	a = buffer.snextc();
	cout << a << endl;

	a = buffer.sbumpc();
	cout << a << endl;

}

