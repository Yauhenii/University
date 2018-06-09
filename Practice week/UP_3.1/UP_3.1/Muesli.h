#pragma once
#include "Sugar.h"
class Muesli :
	public Sugar
{
protected:
	//fields
	bool gluten;
public:
	//constructors
	Muesli();
	Muesli(int wgt, int sgr, bool gl);
	Muesli(const Muesli& obj);
	//methods
	void setStr(std::string str);
	std::string toStr();
	//operators
	Muesli& operator= (const Muesli& obj);
	friend std::istream& operator>> (std::istream& is, Muesli& obj);
};


