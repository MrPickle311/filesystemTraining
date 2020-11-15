#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace filesystem;

bool fileIsEmpty(istream& file) // fstream mozna przekonwertowac na istream ,ale nie na ifstream
{
	cout << "Checking whether file is open..." << endl;
	if (file.peek() == ifstream::traits_type::eof())
		return true;
	return false;
}

void createFile(path filename,string start_data)
{
	ifstream file(filename, ios_base::in); // jezeli plik o takiej nazwie nie istnieje ,to zostanie utworzony
	//nie uzywaj fstream ,tylko ifstream lub ofsream by rozdzielic operacje i pozbyc sie bledow
	if(file.is_open())
	{
		cout << "Opened a file!" << endl;
		if(fileIsEmpty(file))
		{
			file.close();
			ofstream file(filename,ios_base::app);
			file << start_data;
		}
		else
		{
			file.close();
			ofstream file(filename,ios_base::app);
			file << "\t" <<start_data;
		}
	}
	else cout << "Cannot open a file " << endl;
}


void filesystemMain()
{
	path a = current_path();

	path root = a.root_path(); // zwraca sciezke ,ktora symbolizuje podstawe sciezki systemowej np. "/" lub "C:\"
	path parent = a.parent_path(); // zwraca sciezke do katalogu nad tym katalogiem
	cout << a << endl;
	cout << a.filename().relative_path() << endl; // sciezka wzgledem tego katalogu,ale wartosc zwrocone obejmuje rowniez ten katalog
	cout << root << endl;
	cout << parent << endl;
	path file_path = "sandbox1/file.txt";
	if (!create_directory(file_path.parent_path()))
	{
		cout << "The subdir : " << file_path .parent_path() << " hasn't created";
		if(file_path.has_filename()) //sprawdza ,czy sciezka pliku zawiera w sobie nazwe pliku
		{							 //ogolnie w bibliotece filesystem wszystkie metody zaczynajace sie na has_... sprawdzaja ,czy n
			if (exists(file_path))   // dany fragment lancucha istnieje
			{
				cout << ", because dir and file exists\n";
				createFile(file_path,"next text");
			}
			else if (exists(file_path.parent_path()))
			{
				cout << " because dir exisits and file : " << file_path.filename() << " has been created\n";
				createFile(file_path,"Hello file!");
			}
		}
		cout << "with : " << file_path.filename().extension() << " extension"; // extension zwraca rozszerzenie nazwy pliku
		cout << "\nand name :" << file_path.filename().stem() << '\n'; // stem zwraca temat nazwy pliku

		cout << "\n TESTOWANIE MODYFIKATOROW" << endl << endl;

		file_path.replace_extension("dat");
		cout << file_path << endl;
		file_path.replace_extension(".dat");
		cout << file_path << endl;
		//to ,czy damy . przed dat nie ma znaczenia

		file_path.replace_filename("data");
		cout << file_path << endl;
		file_path.replace_filename("data.dat");
		cout << file_path << endl;
		// jak widac ta funkcja calkowicie zmienia nazwe pliku, zostanie podmieniony caly czlon ,razem z rozszerzeniem

		cout << "\n TESTOWANIE ROZNYCH TYPOW SCIEZEK \n" << endl;

		file_path.remove_filename();
		//jak widac roznica miedzy absolute ,a canonical jest taka ,ze cononical przeksztalca wszelkie ..
		//tak ,by w efekcie dostac zmieniona sciezke nie zawierajaca .. ,tylko bezposrednia sciezke dostepu
		//czyli przedstawia sciezke jawnie , poruszajac sie tylko w przod , gdzie absolute pozwala reprezentowac
		//ruch wstecz za pomoca ..
		file_path += "..";
		cout << absolute(file_path) << endl;
		cout << canonical(file_path) << endl;
		cout << relative(file_path) << endl;
		cout << endl;

	}
}


