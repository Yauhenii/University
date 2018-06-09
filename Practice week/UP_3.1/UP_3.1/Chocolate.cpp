#include "Chocolate.h"

Chocolate::Chocolate()
{
	this->weight = 50;
	this->sugar = 20;
	this->cocoa = 50;
}

Chocolate::Chocolate(int wgt, int sgr, int cca)
{
	this->weight = wgt;
	this->sugar = sgr;
	this->cocoa = cca;
}

Chocolate::Chocolate(const Chocolate& obj)
{
	this->weight = obj.weight;
	this->sugar = obj.sugar;
	this->cocoa = obj.cocoa;
}

Chocolate& Chocolate::operator= (const Chocolate& obj)
{
	if (this != &obj)
	{
		this->weight = obj.weight;
		this->sugar = obj.sugar;
		this->cocoa = obj.cocoa;
	}
	return *this;
}

void Chocolate::setStr(std::string str)
{
	std::stringstream ss(str);
	ss >> this->weight;
	ss >> this->sugar;
	ss >> this->cocoa;
}

std::string Chocolate::toStr()
{
	std::stringstream ss;
	ss << "Chocolate, w: " << this->weight << " s: " << this->sugar << "% c: " << this->cocoa <<"%";
	return ss.str();
}