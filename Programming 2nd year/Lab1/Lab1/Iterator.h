#pragma once

template <typename T>
class Iterator
{

private:
	T * pointer;
public:
	//Constructors
	Iterator()
	{
		pointer = nullptr;
	}
	Iterator(T* ptr)
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
		Iterator it = *this;
		--it;
		return it;
	}
	Iterator next() {
		Iterator it = *this;
		++it;
		return it;
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
	T& operator *()
	{
		return *pointer;
	}
	T* operator ->()
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
		Iterator<T> temp = *this;
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
		Iterator<T> temp = *this;
		--*this;
		return temp;
	}
};

