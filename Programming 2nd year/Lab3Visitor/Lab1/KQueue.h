#pragma once
#include <iostream>
#include <initializer_list>
#include "Iterator.h"

#define DEFAULT_SIZE 10

template< typename M >
void swap_v(M& , M& );
template< typename M >
void swap_p(M* , M* );

template<typename T >
class KQueue {
	//friend class Iterator<T>;
private:
	T * queue, *begin, *end;
	int size;
	int quantity;
public:
	typedef Iterator<T> iterator;
	//Constructors
	KQueue(int sz = DEFAULT_SIZE)
	{
		queue = new T[sz];
		size = sz;
		quantity = 0;
		begin = this->queue;
		end = this->queue;
	}
	KQueue(const KQueue &obj)
	{
		queue = new T[obj.size];
		size = obj.size;
		quantity = obj.quantity;
		for (int i = 0; i < quantity; i++)
			queue[i] = obj.queue[i];
		begin = this->queue;
		end = &this->queue[obj.quantity-1];
		end++;
	}
	KQueue(KQueue &&obj)
	{
		queue = obj.queue;
		begin = obj.begin;
		end = obj.end;
		obj.queue = nullptr;
		obj.begin = nullptr;
		obj.end = nullptr;
		size = obj.size;
		quantity = obj.quantity;
	}
	KQueue(std::initializer_list<T> list)
	{
		size = 2*list.size();
		quantity = list.size();
		queue = new T[size];
		int i = 0;
		for ( auto ptr = list.begin(); ptr < list.end(); ptr++, i++)
		{
			queue[i] = *ptr;
		}
		begin = queue;
		end = &queue[quantity - 1];
		end++;
	}
	//Methods
	bool isEmpty()
	{ 
		return this->quantity==0 ? true : false; 
	} //
	int getSize()
	{ 
		return this->size;
	} //
	int getQuantity()
	{ 
		return this->quantity;
	} //
	void clear() //
	{
		delete[] this->queue;
		this->quantity = 0;
		this->begin = this->queue;
		this->end = this->queue;
		this->queue = new T[this->size];
	}
	T front()
	{
		if (quantity != 0)
		{
			return *this->begin;
		}
		else
		{
			throw "Queue is empty!";
		}
	} 
	T back()
	{
		if (quantity != 0)
		{
			return *(this->end - 1);
		}
		else
		{
			throw "Queue is empty!";
		}
	} 
	void push(T value) //
	{
		this->quantity++;
		if (this->size > this->quantity)
		{
			*this->end = value; //this->queue[this->quantity]=value;
			this->end++;
		}
		else
		{
			size = size * 2; //double mem
			T* newqueue = new T[size];
			for (int i = 0; i<quantity-1; i++) //replace elements
			{
				newqueue[i] = *begin;
				begin++;
			}
			newqueue[quantity-1] = value;
			delete [] this->queue;
			this->queue = newqueue;
			end = &newqueue[quantity-1];
			end++;
			begin = newqueue;
		}
	}
	template<typename... ARGS>
	void emplace(ARGS&&... args) {
		T value = T(std::forward<ARGS>(args)...);
		this->quantity++;
		if (this->size >= this->quantity)
		{
			*this->end = value; //this->queue[this->quantity]=value;
			this->end++;
		}
		else
		{
			size = size * 2; //double mem
			T* newqueue = new T[size];
			for (int i = 0; i<quantity - 1; i++) //replace elements
			{
				newqueue[i] = *begin;
				begin++;
			}
			newqueue[quantity - 1] = value;
			delete this->queue;
			this->queue = newqueue;
			end = &newqueue[quantity - 1];
			end++;
			begin = newqueue;
		}
	}
	void pop()
	{
		if (quantity != 0)
		{
			if (quantity == 1)
			{
				delete[] queue;
				queue = new T[size];
				quantity = 0;
				begin = this->queue;
				end = this->queue;
			}
			else
			{
				this->begin++;
				this->quantity--;
			}
		}
		else
		{
			throw "Queue is empty!";
		}
	}
	void swap(KQueue& obj)
	{
		T* tempqueue = queue;
		queue = obj.queue;
		obj.queue = tempqueue;
		T* tempbegin = begin;
		begin = obj.begin;
		obj.begin = tempbegin;
		T* tempend = end;
		end = obj.end;
		obj.end = tempend;
		/*swap_p(queue, obj.queue);
		swap_p(begin, obj.begin);
		swap_p(end, obj.end);*/
		swap_v(size, obj.size);
		swap_v(quantity, obj.quantity);
	}
	//Operators
	bool operator== (const KQueue& obj)
	{
		if(quantity!=obj.quantity)
			return false;
		else
		{
			for (int i = 0; i < quantity; i++)
				if (queue[i] != obj.queue[i])
					return false;
		}
		return true;
	}
	bool operator!= (const KQueue& obj)
	{
		return !(*this == obj);
	}
	KQueue& operator=(const KQueue& obj)
	{
		if (this == &KQueue)
		{
			return *this;
		}
		queue = new T[obj.size];
		size = obj.size;
		quantity = obj.quantity;
		begin = this->queue;
		end = &this->queue[obj.quantity];
		end++;
		for (int i = 0; i < size; i++)
			queue[i] = obj[i];
		return *this;
	}
	KQueue& operator=(KQueue&& obj)
	{
		if (this == &KQueue)
		{
			return *this;
		}
		queue = obj.queue;
		begin = obj.begin;
		end = obj.end;
		obj.queue = nullptr;
		obj.begin = nullptr;
		obj.end = nullptr;
		size = obj.size;
		quantity = obj.quantity;
		return *this;
	}
	KQueue& operator+=(const KQueue& obj) 
	{
		int newquantity = quantity + obj.quantity;
		size = newquantity * 2;
		T* newqueue = new T[size];
		for (int i = 0; i < quantity; i++)
		{
			newqueue[i] = queue[i];
		}
		for (int i = 0; i < obj.quantity; i++)
		{
			newqueue[i+quantity] = obj.queue[i];
		}
		quantity = newquantity;
		delete[] queue;
		queue = newqueue;
		end = &queue[newquantity];
		begin = newqueue;
		return *this;
	}
	template <typename M>
	friend std::ostream& operator<< (std::ostream& os, KQueue<M>& obj);
	template <typename M>
	friend std::istream& operator>> (std::istream& is, KQueue<M>& obj);
	//Iterators
	Iterator<T> beginIt()
	{
		return Iterator<T>(this->begin);
	}
	Iterator<T> endIt()
	{
		return Iterator<T>(this->end + 1);
	}
	//Destructor
	~KQueue()
	{
		delete [] this->queue;
	}
};

template< typename M >
void swap_v(M& val1, M& val2)
{
	M temp = val1;
	val1 = val2;
	val2 = temp;
}
template< typename M >
void swap_p(M* ptr1, M* ptr2)
{
	M* temp = ptr1;
	ptr1 = ptr2;
	ptr2 = temp;
}
template< typename T >
std::ostream& operator<< (std::ostream& os, KQueue<T>& obj)
{
	for (int i = 0; i < obj.quantity; i++)
	{
		os << obj.queue[i] << " ";
	}
	return os;
}
template< typename T >
std::istream& operator>> (std::istream& is, KQueue<T>& obj)
{
	T value;
	is >> value;
	obj.push(value);
	return is;
}
