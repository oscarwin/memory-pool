
#ifndef MEMORY_POOL_CPP
#define MEMORY_POOL_CPP 

template<typename T>
CMemoryPool<T>::CMemoryPool()
:freeList(0){}

template<typename T>
inline typename CMemoryPool<T>::pointer
CMemoryPool<T>::newElement()
{
	pointer result = allocate();
	construct(result);
	return result;
}

template<typename T>
template<class U>
inline typename CMemoryPool<T>::pointer
CMemoryPool<T>::newElement(const U& args)
{
	pointer result = allocate();
	construct(result, args);
	return result;
}

template<typename T>
inline void
CMemoryPool<T>::deleteElement(pointer p)
{
	if(p != 0)
	{
		destroy(p);
		deallocate(p);
	}
}


template<typename T>
typename CMemoryPool<T>::pointer 
CMemoryPool<T>::allocate()
{
	list_pointer result = freeList;

	//从freeList里面寻找是否有空闲的内存
	if(0 == result)
	{
		freeList = reinterpret_cast<list_pointer>(fillList(sizeof(obj)));
		result = freeList;
	}

	freeList = result->next;
	return reinterpret_cast<pointer>(result);
}

template<typename T>
inline void 
CMemoryPool<T>::deallocate(pointer p)
{
	if(p != 0)
	{
		reinterpret_cast<list_pointer>(p)->next = freeList;
		freeList = reinterpret_cast<list_pointer>(p);
	}
}

template<typename T>
inline void
CMemoryPool<T>::construct(pointer p)
{
	new(p) T();
}

template<typename T>
template<class U>
inline void
CMemoryPool<T>::construct(pointer p, const U& args)
{
	//implacement new 实现在已经分配的内存上调用构造函数
	new(p) T(args);
}

template<typename T>
inline void
CMemoryPool<T>::destroy(pointer p)
{
	p->~T();
}

template<typename T>
void* CMemoryPool<T>::fillList(size_t n)
{
	//一次分配多个链表节点，减少分配的频率
	int nodeNum = 10;

	char* chunk = chunkAlloc(n, nodeNum);
	
	void* result;
	list_pointer curPointer, nextPointer;

	result = reinterpret_cast<void*>(chunk);

	if(1 == nodeNum)
	{
		(reinterpret_cast<list_pointer>(chunk))->next = 0;
		return result;
	}

	nextPointer = reinterpret_cast<list_pointer>(chunk + n);

	for(int i = 1; ; ++i)
	{
		curPointer = nextPointer;
		nextPointer = reinterpret_cast<list_pointer>(reinterpret_cast<char*>(nextPointer) + n);
		if(nodeNum - 1 == i)
		{
			curPointer->next = 0;
			break;
		}
		else
		{
			curPointer->next = nextPointer;
		}
	}

	return result;
}

template<typename T>
char* CMemoryPool<T>::chunkAlloc(size_t n, int & nodeNum)
{
	char* result;
	size_t totalBytes = n * nodeNum;
	size_t bytesRemain = endPool - startPool;

	if(bytesRemain >= totalBytes)
	{
		result = startPool;
		startPool += totalBytes;
		return result;
	}
	else if(bytesRemain >= n)//memory pool cannot contains all, but can alloc some
	{
		nodeNum = bytesRemain / n;
		totalBytes = n * nodeNum;
		result = startPool;
		startPool += totalBytes;
		return result;
	}
	else  //one also connot alloc
	{
		//submit 2 times of totalBytes memory
		size_t bytesToGet = totalBytes * 2;
		startPool = reinterpret_cast<char*>(malloc(bytesToGet));
		
		if(0 == startPool) //if failure, throw exception
		{
			throw std::bad_alloc();
		}
		else
		{
			endPool = startPool + bytesToGet;
			return chunkAlloc(n, nodeNum);  //recall the function to fix nodeNum
		}
	}
}

#endif
