﻿#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace filesystem;

bool fileIsEmpty(istream& file) // fstream mozna przekonwertowac na istream ,ale nie na ifstream
{								//to tak, przeciez fstream dziedziczy po istream, a nie po ifstream
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

void copyTests(path file_path)
{
	cout << "\n TESTY KOPIOWANIA\n" << endl;
	//mniej wiecej tak powinien wygladac kopiowanie , fajne wysokopoziomowe podejscie
	//pamietaj ,ze jesli bedziesz operaowac na dwoch sciezkach wywodzacych sie z jednej , to zawsze kopiuj sciezke
	try
	{
		path new_file = file_path;
		new_file.remove_filename();
		new_file /= "file2.txt";
		cout << "New file path :"<< new_file << endl;
		//copy_file(file_path,new_file); wywala wyjatek jesi plik o takiej samej sciezce istnieje
		copy_file(file_path,new_file,copy_options::skip_existing);//powinno grac, jezeli plik o takiej samej sciezce istnieje
		//to ta flaga sprawia ,ze istniejacy plik nie zostanie nadpisany, istnieje tez odwrotna flaga, preferuj wywolywanie funkcji
		//z flagami,badz sprawdzaj to jakos
		//sa jeszcze flagi do kopiowania katalogow
		cout << "File has benn copied succesfully " << endl;
	}
	catch (filesystem_error& e)
	{
		cout << "Error with copying : "<< e.what() << endl;
	}
	cout << "Kopiowanie folderow" << endl;
	file_path.remove_filename();
	path p = file_path;
	p /= "dir2";
	file_path /= "dir";
	const auto copyFlags {/*copy_options::directories_only | */ copy_options::recursive | copy_options::update_existing};
	//chyba widac do czego te flagi sluza, recursive - kopiuje wszystkie podfoldery i ich pliki
	try
	{
		copy(file_path,p,copyFlags);
		static_cast<void>(system("tree")); // ciekawe wywolanie , pokazuje drzewo katalogowe
	}
	catch (filesystem_error& e)
	{
		cout << "Error with copying folder : " << e.what() << endl;
		cout << "With path : " << e.path1() << " and " << e.path2() << endl;
		//dodatkowo ta klasa bledu moze zawierac sciezki :
		//sciezka zrodlowa = e.path1()
		//sciezka docelowa = e.path2()
	}


}


void symLinkTests(path file_path)//no dobra, to mi sie crashuje ,moze cos z uprawnieniami
								 //i tak nie jest mi to potrzebne specjalnie
{
	cout << "\n TESTOWANIE LINKOW SYMBOLICZNYCH\n" << endl;
	path dir_path = file_path;
	if(dir_path.has_filename())
		dir_path.remove_filename();
	path p_dir = dir_path += "..";
	cout << "dir_path = " << dir_path << endl;
	cout << "p_dir = " << p_dir <<endl;

	create_symlink(p_dir,file_path);
	//create_directory_symlink(p_dir,file_path);

}

void filesystemMain()
{
	path a = current_path();

	path root = a.root_path(); // zwraca sciezke ,ktora symbolizuje podstawe sciezki systemowej np. "/" lub "C:\"
	path parent = a.parent_path(); // zwraca sciezke ,ktora
	cout << a << endl;
	cout << a.filename().relative_path() << endl; // sciezka wzgledem tego katalogu,ale wartosc zwrocone obejmuje rowniez ten katalog
	cout << "root_path : " << root << endl;
	cout << "parent_path : " << parent << endl;
	current_path(a);//ustawiam z powrotem obecna sciezke na ta ,ktora mialem wczesniej
	//tutaj uwazaj na wyjatkowe sytuacje
	path file_path = "sandbox1/file.txt";
	cout << "Rozmiar pliku file.txt to : " << file_size(file_path) << endl; // zwraca rozmiar w bajtach
	//symLinkTests(file_path);
	copyTests(file_path);
	cout << "\n TEST TWORZENIA NOWEGO PLIKU\n" << endl;
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

		cout << "\n TESTOWANIE OPERATOROW KONTATENCJI" << endl;

		path pth = file_path.remove_filename();

		cout << "pth = " << pth << endl;
		pth += "aaa";
		cout << "pth = " << pth << endl;
		pth += "bbb";
		cout << "pth = " << pth << endl;

		pth.remove_filename();
		cout << "pth = " << pth << endl;

		path pth2 = pth;

		cout << "pth2 " << pth2<<endl;
		pth2 /= "/aaa";
		cout << "pth2 " << pth2<<endl;

		cout << "pth = " << pth << endl;
		pth /= "aaa";
		cout << "pth = " << pth << endl;
		pth /= "bbb";
		cout << "pth = " << pth << endl;

		//jak widac operator += nie dodaje znaku separatora / przed dodawanym stringiem
		//operator /= dodaje operator separacji / przed dodawanym stringiem
		//dodatkowo jezeli sami zapewnimy taki znak w dodawanym lancuchu(patrz pth2) ,to zostanie on zignorowany

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


