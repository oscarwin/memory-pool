# MemoryPool
implement a memoryPool by C++

实现一个简单的内存池
newElement(const U& args)函数用于创建一个对象，并调用构造函数
void deleteElement(pointer p)函数先调用析构函数，然后回收内存到内存池

使用方法见testMemoryPool.cpp文件
