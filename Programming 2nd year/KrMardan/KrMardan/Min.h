#pragma once
#include "Visitor.h"
 
class Min : public Visitor
{
	std::string min;
	int length;
public:
	//Constructor
	Min()
	{
		length = min.max_size();
	}
	void visit(std::string &ref) override
	{
		if (ref.length() < length)
		{
			length = ref.length();
			min = ref;
		}
	}
	std::string getMin()
	{
		return min;
	}
};