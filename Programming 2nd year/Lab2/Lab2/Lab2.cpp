// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

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

template <class Arg>
Arg min(Arg arg) 
{
	return arg;
}
template <class Arg, class ... Other>
auto min(Arg arg, Other ... other)
{
	return arg + min(other...);
}

int main()
{
	cout << sum(3.0, 'A', 5) << endl;
	system("pause");
    return 0;
}

