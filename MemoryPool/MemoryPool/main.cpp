#include "MemoryPool.h"
#pragma pack(push,1)
struct test_value
{
	double gety() { return y; }
	double y;
	int x;
	char z;
};
#pragma pack(pop)

class Test : public MemoryPool<Test> 
{
public:
	Test() { }//test_value* test = new test_value(); }
	~Test() {}
	test_value test;
};

int main()
{
	int size = sizeof(Test);
	Test* test = new Test[10];
	
	//Test* test2 = new Test();

	////delete test;
	//Test* test3 = new Test();

	//MemoryPool<Test>::End();

	//int size = sizeof(char*);
	//printf("%d", size);
	//int a;
	//double* arr = new double[50];
	//void* befor = arr;
	//char* next = (char*)arr + sizeof(int);
	//char** value = &next;
	//memcpy(arr, value, sizeof(char*));
	////*arr = (int)*value;
	//char buf[100];
	//ZeroMemory(buf, 100);
	//a = (int)*value;
	//printf("%p,%p,%p,%p\n", &arr,arr,*arr,*value);


	//int* a = new int((int)*value);
	//int* ptr=new int();
	//memcpy(ptr, a, sizeof(int));
	//
	//printf("%p", a);

	//int x = 10;
	//int* y = &x;
	//int** z = &y;

	//printf("%p %p %p %p %d", &x, y, z, *z, **z);
}
