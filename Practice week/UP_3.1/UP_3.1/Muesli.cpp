#include "Muesli.h"

Muesli::Muesli()
{
	this->weight = 50;
	this->sugar = 10;
	this->gluten = 1;
}

Muesli::Muesli(int wgt, int sgr, bool gln)
{
	this->weight = wgt;
	this->sugar = sgr;
	this->gluten = gln;
}

Muesli::Muesli(const Muesli& obj)
{
	this->weight = obj.weight;
	this->sugar = obj.sugar;
	this->gluten = obj.gluten;
}

Muesli& Muesli::operator= (const Muesli& obj)
{
	if (this != &obj)
	{
		this->weight = obj.weight;
		this->sugar = obj.sugar;
		this->gluten = obj.gluten;
	}
	return *this;
}

void Muesli::setStr(std::string str)
{
	std::stringstream ss(str);
	ss >> this->weight;
	ss >> this->sugar;
	ss >> this->gluten;
}

std::string Muesli::toStr()
{
	std::stringstream ss;
	ss << "Museli, w:" << this->weight << " s:" << this->sugar << "% gl: ";
	if (this->gluten)
		ss << "yes";
	else
		ss << "no";
	return ss.str();
}