#pragma once
#include <string>

class Visitor
{
public:
	virtual void visit(std::string &ref) = 0;
};