#pragma once
#include "KQueue.h"
#include <string>

#define INT_CONST 0
#define STRING_CONST 1

class View;
class Model
{
private:
	int type;
	std::pair<KQueue<int>, KQueue<std::string>> queue;
public:
	typedef KQueue<int>::iterator iteratorInt;
	typedef KQueue<std::string>::iterator iteratorString;
	//Methods
	void setType(int iType)
	{
		if (iType == INT_CONST || iType == STRING_CONST)
		{
			type = iType;
		}
	}
	int getType()
	{
		return type;
	}
	bool isStringType()
	{
		return type == STRING_CONST ? true : false;
	}
	bool isIntType()
	{
		return type == INT_CONST ? true : false;
	}
	bool isEmpty(int type)
	{
		if (type == STRING_CONST)
		{
			return queue.second.isEmpty();
		}
		else
		{
			return queue.first.isEmpty();
		}
	}
	void clear(int type)
	{
		if (type == STRING_CONST)
		{
			queue.second.clear();
		}
		else
		{
			queue.first.clear();
		}
	}
	void pop(int type)
	{
		if (type == STRING_CONST)
		{
			if (!queue.second.isEmpty())
				queue.second.pop();
		}
		else
		{
			if (!queue.first.isEmpty())
				queue.first.pop();
		}
	}
	int getSize(int TYPE)
	{
		if (TYPE == STRING_CONST)
		{
			return queue.second.getSize();
		}
		else
		{
			return queue.first.getSize();
		}
	}
	int getQuantity(int TYPE)
	{
		if (TYPE == STRING_CONST)
		{
			return queue.second.getQuantity();
		}
		else
		{
			return queue.first.getQuantity();
		}
	}
	int frontInt()
	{
		return queue.first.front();
	}
	std::string frontString()
	{
		return queue.second.front();
	}
	int backInt()
	{
		return queue.first.back();
	}
	std::string backString()
	{
		return queue.second.back();
	}
	iteratorInt beginItInt()
	{
		return queue.first.beginIt();
	}
	iteratorString beginItString()
	{
		return queue.second.beginIt();
	}
	iteratorInt endItInt()
	{
		return queue.first.endIt();
	}
	iteratorString endItString()
	{
		return queue.second.endIt();
	}
	void accept(Visitor& v)
	{
		queue.second.accept(v);
		queue.first.accept(v);
	}
	void push(int value)
	{
		queue.first.push(value);
	}
	void push(std::string value)
	{
		queue.second.push(value);
	}
	//Operators

	//Destructor
	//~Model();
};

