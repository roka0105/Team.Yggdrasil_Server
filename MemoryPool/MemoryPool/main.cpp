
#include "MemoryPool.h"
class Test : public MemoryPool<Test> 
{
public:
	Test() {}
	~Test() {}
	double y;
	int x;
	char z;
};
//void* operator new(size_t size)
//{
//
//}
int main()
{
	
	//Test* test = new Test();
	void* arr = new int(sizeof(int) * 50);
	void* befor = arr;
	int* next = (int*)arr + sizeof(int);
	int** value = &next;
	memcpy(arr, value, sizeof(int*));
	char buf[100];
	ZeroMemory(buf, 100);
	
	printf("%p,%d", buf,buf);
}