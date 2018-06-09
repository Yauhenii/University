#include "Jelly.h"

Jelly::Jelly()
{
	this->weight = 50;
	this->sugar = 20;
	this->color = "white";
}

Jelly::Jelly(int wgt, int sgr, std::string clr)
{
	this->weight = wgt;
	this->sugar = sgr;
	this->color = clr;
}

Jelly::Jelly(const Jelly& obj)
{
	this->weight = obj.weight;
	this->sugar = obj.sugar;
	this->color = obj.color;
}

Jelly& Jelly::operator= (const Jelly& obj)
{
	if (this != &obj)
	{
		this->weight = obj.weight;
		this->sugar = obj.sugar;
		this->color = obj.color;
	}
	return *this;
}

void Jelly::setStr(std::string str)
{
	std::stringstream ss(str);
	ss >> this->weight;
	ss >> this->sugar;
	ss >> this->color;
}

std::string Jelly::toStr()
{
	std::stringstream ss;
	ss << "Jelly, w: " << this->weight << " s: " << this->sugar << "% c: "<<this->color;
	return ss.str();
}