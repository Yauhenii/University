// KQueue_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "KQueue.h"
#include "Iterator.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	KQueue<string> queue;
	queue.emplace("My");
	queue.emplace("Baby");
	queue.pop();
	cout << "Quantity:" << queue.getQuantity() << " Size:" << queue.getSize() << endl;
	cout << queue.front() << " "<< queue.back() << endl;
	cout << queue << endl;

	KQueue<string>::iterator it(queue.beginIt());
	it = queue.endIt();
	cout << *it.prev();
	for (; it != queue.endIt(); ++it)
		cout << *it << endl;
	system("pause");
    return 0;
}

