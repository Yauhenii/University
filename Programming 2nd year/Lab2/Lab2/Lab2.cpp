// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream> //or use to_string
#include <tuple>

using namespace std;

#define GETTYPE(x) typeid(x).name()
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
tuple<int, double> calc(int a) {
	return { a, 0.0 };
}

tuple<int, double> calc(double a) {
	return { 0, a };
}

template<typename T>
tuple<int, double> calc(T) {
	return { 0, 0.0 };
}
template <class Arg,class ... Other>
tuple<int, double> calc(Arg arg,Other... other)
{
	return { get<0>(calc(arg)) + get<0>(calc(other...)), get<1>(calc(arg)) + get<1>(calc(other...)) };
}
//Problem 4
template <class Arg>
string ToString(string d, Arg arg)
{
	stringstream s;
	s << arg;
	return s.str();
}
template <class Arg,class ... Other>
string ToString(string d, Arg arg, Other ... other)
{
	stringstream s;
	s << arg;
	return s.str() + d + ToString(d, other...);
}

int main()
{
	cout << "Problem 1: sum=" << sum(3, 4.4, 'b', true, 0xA) << endl;
	cout << "Problem 2: min=" << min(3, 4.4, 'b', true, 0xA) << endl;
	auto tpl = calc(1, 'b', 3.3, "hello", 0xA, true, 14, 13.32, 1000, "tr");
	cout << "Problem 3: sum of int - " << get<0>(tpl) << " sum of double - " << get<1>(tpl) << endl;
	cout << "Problem 4: " << ToString(",", 1, 'b', 3.3, "hello", 0xA, true) << endl;
	system("pause");
    return 0;
}

