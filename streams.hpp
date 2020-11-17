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

using namespace std;
using namespace filesystem;

void fix_out()
{
	path p {"sandbox1/file.txt"};
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

void mixed_buffers()
{
	path p = "sandbox1/file.txt";
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
