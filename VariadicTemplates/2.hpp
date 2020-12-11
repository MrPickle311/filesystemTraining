#pragma once

#include <iostream>
#include <string>

using count = size_t;
using state = size_t;

class Warrior
{
private:
	 state strenght_;
	 count might_nmbr_;
public:
	Warrior():
		strenght_{ 0 },
		might_nmbr_{ 0 }
	{

	}
	 Warrior(state strenght,
		     count might_nmbr): 
		strenght_{ strenght },
		might_nmbr_{ might_nmbr }
	{

	}
	void kill_the_enemies()
	{
		std::cout << "For the emperror!" << std::endl;
	}
};

class Mage
{
private:
	count spell_power_;
	count mana_;
public:
	 Mage(count spell_power,
			      count mana) :
		spell_power_{ spell_power },
		mana_{ mana }
	{

	}
	void spell_a_cast() const
	{
		std::cout << "Fireball!" << std::endl;
	}
};

class Hunter
{
private:
	count arrows_nmbr_;
	state speed_;
public:
	 Hunter(count arrows_nmbr,
					state speed) :
		arrows_nmbr_{ arrows_nmbr },
		speed_{ speed }
	{

	}
	void kill_the_wolf()
	{
		std::cout << "Wolf has been slain" << std::endl;
	}
};

class Lockpick_ability
{
private:
	state lock_experience_;
public:
	Lockpick_ability(state lock_exp) :
		lock_experience_{ lock_exp }
	{

	}
	void unlock_the_lock()
	{
		std::cout << "Unlocked!" << std::endl;
	}
};

template<class... Args>
class Character: public Args...
{
private:
	std::string name_;
public:
	Character(const std::string& name,
			  Args... args):
		Args( args )...,//wywo³anie konstruktorów przekazanych typów
		name_{ name }
	{

	}
};

template< >
class Character< >
{
private:
	std::string name_;
public:
	Character(const std::string& name):
		name_{ name }
	{

	}
};

void spell( Mage const& mage)
{
	mage.spell_a_cast();
}

