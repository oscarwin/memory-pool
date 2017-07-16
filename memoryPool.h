
/*
 * Copyright (c) 2017 Oscarwin, https://github.com/oscarwin
 */

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <new>
#include <cstddef>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>

template<typename T>
class CMemoryPool
{
public:
	typedef T        value_type;
	typedef T*       pointer;
	typedef T&       reference;
	typedef const T* const_pointer;
	typedef const T& const_reference;
	typedef size_t   size_type;

	//constructor function
	CMemoryPool();

    pointer newElement();
	template<class U> pointer newElement(const U& args);
	void deleteElement(pointer p);

	void construct(pointer p);
	template<class U> void construct(pointer p, const U& args);
	void destroy(pointer p);

	pointer allocate();
	void deallocate(pointer p);

	void* fillList(size_t n);

	char* chunkAlloc(size_t n, int& nodeNum);


private:
	union obj
	{
		obj* next;
		value_type clientData;
	};
	typedef union obj  list_type;
	typedef union obj* list_pointer;

	list_pointer freeList;

private:
	static char* startPool;
	static char* endPool;
};

template<typename T> char* CMemoryPool<T>::startPool = 0;
template<typename T> char* CMemoryPool<T>::endPool = 0;

#include "memoryPool.cpp"

#endif
