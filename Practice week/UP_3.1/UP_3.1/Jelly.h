#pragma once
#include "Sugar.h"

class Jelly :
	public Sugar
{
protected:
	//fields
	std::string color;
public:
	//constructors
	Jelly();
	Jelly(int wgt, int sgr, std::string clr);
	Jelly(const Jelly& obj);
	//methods
	void setStr(std::string str);
	std::string toStr();
	//operators
	Jelly& operator= (const Jelly& obj);
	friend std::istream& operator>> (std::istream& is, Jelly& obj);
};

