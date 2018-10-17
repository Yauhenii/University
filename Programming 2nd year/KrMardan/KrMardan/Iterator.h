#pragma once
#include <string>

class Iterator
{

private:
	std::string * pointer;
public:
	//Constructors
	Iterator()
	{
		pointer = nullptr;
	}
	Iterator(std::string* ptr)
	{
		pointer = ptr;
	}
	Iterator(const Iterator &it)
	{
		if (this != &it)
		{
			pointer = it.pointer;
		}
	}
	//Methods
	Iterator prev() {
		this->pointer--;
		return *this;
	}
	Iterator next() {
		this->pointer++;
		return *this;
	}
	//Operators
	bool operator ==(const Iterator& it)
	{
		return pointer == it.pointer;
	}
	bool operator !=(const Iterator& it)
	{
		return !(*this == it);
	}
	Iterator& operator =(const Iterator &it)
	{
		if (this != &it)
		{
			pointer = it.pointer;
		}
		return *this;
	}
	std::string& operator *()
	{
		return *pointer;
	}
	std::string* operator ->()
	{
		return pointer;
	}
	Iterator& operator ++()
	{
		++pointer;
		return *this;
	}
	Iterator operator ++(int)
	{
		Iterator temp = *this;
		++*this;
		return temp;
	}
	Iterator& operator --()
	{
		--pointer;
		return *this;
	}
	Iterator operator --(int)
	{
		Iterator temp = *this;
		--*this;
		return temp;
	}
};