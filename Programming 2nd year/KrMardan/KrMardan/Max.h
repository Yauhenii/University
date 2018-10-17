#pragma once
#include "Visitor.h"

class Max : public Visitor
{
	std::string max;
	int length;
public:
	//Constructor
	Max()
	{
		max = "";
		length = max.length();
	}
	void visit(std::string &ref) override
	{
		if (ref.length() >= length)
		{
			length = ref.length();
			max = ref;
		}
	}
	std::string getMax()
	{
		return max;
	}
};