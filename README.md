# MemoryPool
implement a memoryPool by C++

在学习STL源码时，了解了allocator类，于是打算自己实现一个简单的内存池

对于用户主要使用两个接口：
```
newElement();
newElement(const U& args);
```
该函数用于创建一个对象，并调用构造函数，最后返回对象指针。有带参数版本和无参数版本。

```
void deleteElement(pointer p);
```
该函数先调用析构函数，然后回收内存到内存池；

使用方法见testMemoryPool.cpp文件

在学习写的过程中加深了模板的理解，和内存分配和构造分离的方法。
