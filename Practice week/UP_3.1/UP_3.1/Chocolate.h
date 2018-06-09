#pragma once
#include "Sugar.h"
class Chocolate :
	public Sugar
{
protected:
	//fields
	int cocoa;
public:
	//constructors
	Chocolate();
	Chocolate(int wgt, int sgr, int cca);
	Chocolate(const Chocolate& obj);
	//methods
	void setStr(std::string str);
	std::string toStr();
	//operators
	Chocolate& operator= (const Chocolate& obj);
	friend std::istream& operator>> (std::istream& is, Chocolate& obj);
};

