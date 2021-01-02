#pragma once

#include <iostream>

class Storable
{
public:
	Storable(const std::string& s):
		data{s}
	{}
	virtual std::string get_file() = 0;
	virtual void read()  = 0;
	virtual void write() = 0;
	virtual ~Storable() {}
protected:
	std::string data;

};

class Transmitter:
		public virtual Storable
{
	// Storable interface
public:
	Transmitter(const std::string& s):
		Storable{s}
	{}
	virtual std::string get_file() override
	{
		return "Transmitter get_file";
	}
	virtual void read() override
	{
		std::cout << "Transmitter read" << std::endl;
	}
	virtual void write() override
	{
		data = "ABBA";
		std::cout << "Transmitter write "<< data <<std::endl;
	}
};

class Receiver
		:public virtual Storable
{
	// Storable interface
public:
	Receiver(const std::string& s):
		Storable{s}
	{}
	virtual std::string get_file() override
	{
		return "Receiver get_file";
	}
	virtual void read() override
	{
		std::cout << "Receiver read" << std::endl;
	}
	virtual void write() override
	{
		std::cout << "Receiver write "<< data << std::endl;
	}
};


class Radio:
		public Receiver,
		public Transmitter
{
	// Storable interface
public:
	Radio(const std::string& s):
		Storable{s},			//Klasa najbardziej pochodna musi jawnie inicjalizować klasę wirtualną
		Receiver{s},
		Transmitter{s}
	{}
	virtual std::string get_file() override
	{
	}
	virtual void read() override
	{
	}
	virtual void write() override
	{
		std::cout << data << std::endl;
		Transmitter::write();
		Receiver::write();
	}
};
///Taka konfiguracja klas sprawi ,że będzie tylko jeden
/// obiekt Storable

void operations()
{
	Radio r{"gugu"};
	r.write();
}
