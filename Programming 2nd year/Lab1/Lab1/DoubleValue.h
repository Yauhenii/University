#pragma once
#include "Visitor.h"
class DoubleValue : public Visitor
{
public:
	void visit(int &ref) override
	{
		ref *= 2;
	}
	void visit(std::string &ref) override 
	{
		ref = ref + " " + ref;
	}
	void visit(double &ref) override 
	{
		ref *= 2;
	}
};

