#pragma once
#include <iostream>
#include <string>
#include <sstream>

class Sugar
{
protected:
	//fields
	int weight;
	int sugar;
public:
	//constructors
	Sugar();
	Sugar(int wgt, int sgr);
	Sugar(const Sugar& obj);
	//methods
	int getWeight();
	int getSugar();
	virtual void setStr(std::string str) = 0; 
	virtual std::string toStr() = 0;
	//operators
	Sugar& operator= (const Sugar& obj);
	friend std::istream& operator>> (std::istream& is, Sugar& obj);
};

