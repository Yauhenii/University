#pragma once

#pragma once
#include <iostream>
#include <sstream>
//Iterator
#include "iterator.h"
//Visitor
#include "Min.h"
#include "Max.h"

#define DEFAULT_SIZE 10

class Model
{
private:
	std::string * arr, *ptrBegin, *ptrEnd; //array, begin and end pointers, size and quantity
	int size;
	int quantity;
public:
	typedef Iterator iterator;
	//Constructors
	Model(int sz = DEFAULT_SIZE) //default and with parameter
	{
		arr = new std::string[sz];
		size = sz;
		quantity = 0;
		ptrBegin = this->arr;
		ptrEnd = this->arr;
	}
	Model(const Model &obj) //copy
	{
		arr = new std::string[obj.size];
		size = obj.size;
		quantity = obj.quantity;
		for (int i = 0; i < quantity; i++)
			arr[i] = obj.arr[i];
		ptrBegin = arr;
		ptrEnd = &arr[obj.quantity - 1];
	}
	//Methods
	bool isEmpty()
	{
		return quantity == 0 ? true : false;
	}
	void push_back(std::string value)
	{
		quantity++;
		if (size > quantity)
		{
			*ptrEnd = value;
			ptrEnd++;
		}
		else
		{
			size = size * 2; //double mem
			std::string* newqueue = new std::string[size];
			for (int i = 0; i<quantity - 1; i++)
			{
				newqueue[i] = *ptrBegin;
				ptrBegin++;
			}
			newqueue[quantity - 1] = value;
			delete[] arr;
			arr = newqueue;
			ptrEnd = &newqueue[quantity - 1];
			ptrEnd++;
			ptrBegin = newqueue;
		}
	}
	std::string pop_back()
	{
		if (!isEmpty())
		{
			std::string value = *(ptrEnd-1);
			if (quantity == 1)
			{
				delete[] arr;
				arr = new std::string[size];
				quantity = 0;
				ptrBegin = this->arr;
				ptrEnd = this->arr;
			}
			else
			{
				this->ptrEnd--;
				this->quantity--;
			}
			return value;
		}
		else
		{
			throw "Queue is empty!";
		}
	}
	std::string pop_front()
	{
		if (!isEmpty())
		{
			std::string value = *ptrBegin;
			if (quantity == 1)
			{
				delete[] arr;
				arr = new std::string[size];
				quantity = 0;
				ptrBegin = this->arr;
				ptrEnd = this->arr;
			}
			else
			{
				this->ptrBegin++;
				this->quantity--;
			}
			return value;
		}
		else
		{
			throw "Queue is empty!";
		}
	}
	void accept(Visitor &v)
	{
		for (iterator it = begin(); it != end(); it.next())
			v.visit(*it);
	}
	//Iterators
	Iterator begin()
	{
		return Iterator(this->ptrBegin);
	}
	Iterator end()
	{
		return Iterator(this->ptrEnd);
	}
	//Destructor
	~Model()
	{
		delete[] arr;
	}
};

