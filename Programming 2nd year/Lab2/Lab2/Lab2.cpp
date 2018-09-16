// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream> //or use to_string

using namespace std;

//Problem 1
template <class Arg>
Arg sum(Arg arg)
{
	return arg;
}
template <class Arg,class ... Other>
auto sum(Arg arg, Other ... other)
{
	return arg + sum(other...);
}
//Problem 2
template <class Arg>
Arg min(Arg arg) 
{
	return arg;
}
template <class Arg, class ... Other>
auto min(Arg arg, Other ... other)
{
	return arg < min(other...) ? arg : min(other...);
}
//Problem 3

//Problem 4
template <class Arg>
string ToString(Arg arg)
{
	stringstream s;
	s << arg;
	return s.str();
}
template <class Arg,class ... Other>
string ToString(Arg arg, Other ... other)
{
	stringstream s;
	s << arg;
	return s.str() + ToString(other...);
}

int main()
{
	cout << ToString(3.4,true, 'A', -5) << endl;
	system("pause");
    return 0;
}

