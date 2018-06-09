#include "Sugar.h"

Sugar::Sugar()
{
	this->weight = 0;
	this->sugar = 1;
}

Sugar::Sugar(int wgt, int sgr)
{
	this->weight = wgt;
	this->sugar = sgr;
}

Sugar::Sugar(const Sugar& obj)
{
	this->weight = obj.weight;
	this->sugar = obj.sugar;
}

Sugar& Sugar::operator= (const Sugar& obj)
{
	if (this != &obj)
	{
		this->weight = obj.weight;
		this->sugar = obj.sugar;
	}
	return *this;
}

 int Sugar::getWeight()
{
	 return this->weight;
}

 int Sugar::getSugar()
 {
	 return this->sugar;
 }


