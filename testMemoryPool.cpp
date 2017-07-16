#include "memoryPool.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	CMemoryPool<int> alloc;
	int* a;
	try
	{
		a = alloc.newElement(1);
	}
	catch(std::bad_alloc& e)
	{
		cout << "out of space" << endl;
		return -1;
	}
	
	cout << *a << endl;

	alloc.deleteElement(a);
	return 0;
}
