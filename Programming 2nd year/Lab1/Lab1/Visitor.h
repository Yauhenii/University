#pragma once
#include <string>

class Visitor
{
public:
	virtual void visit(int &ref) = 0;
	virtual void visit(std::string &ref) = 0;
	virtual void visit(double &ref) = 0;
};

