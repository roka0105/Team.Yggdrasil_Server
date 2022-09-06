#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <crtdbg.h>
#include "C_SetCtrlHandler.h"
#include "testclass.h"
//#include "MemoryPool.h"
#include "MemoryPool_2.h"
#pragma pack(push,1)
struct test_value
{
	double gety() { return y; }
	double y;
	int x;
	char z;
};
#pragma pack(pop)
#pragma pack(push,1)
struct test_value2
{
	test_value2()
	{
		x = 0;
		y = 0;
	}
	int x;
	int y;
};
#pragma pack(pop)

class GameObject :public MemoryPool_2
{
public:
	int GetID() { return m_id; }
private:
	int m_id;
	
};
class Object1:public GameObject
{
public:
	int GetA() { return a; }
	int GetB() { return b; }
private:
	int a;
	int b;

};
class Object2 :public GameObject
{
public:
	double GetA() { return a; }
	double GetB() { return b; }
private:
	double a;
	double b;
};

int main()
{
	/*int size = sizeof(Test);
	Test* test = new Test();
	delete test;*/

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

#pragma region MemoryPool_2 Test

	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	int size2 = sizeof(Test2);
	Test* test = new Test();
	Test2* test2_1 = new Test2();
	Test2* test2_2 = new Test2();
	Test2* test2_3 = new Test2();
	Test2* test2_4 = new Test2();
	delete test2_1;
	delete test2_3;
	delete test2_4;
	delete test2_2;

	test2_1 = new Test2();
	test2_2 = new Test2();
	test2_3 = new Test2();
	test2_4 = new Test2();

	MemoryPool_2::End();*/
	
#pragma endregion
#pragma region 메모리 누수 검사 Test
	C_SetCtrlHandler::Create();
	C_SetCtrlHandler::GetInst()->Init();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(159);
	//int* a = new int;
	//int count = 5;
	//while (count!=0)
	//{
	//	printf("d\n");
	//	count--;
	//}
	//_CrtDumpMemoryLeaks();
	//
	testclass::run();
	
	
#pragma endregion
#pragma region
	/*int size1 = sizeof(Object1);
	int size2 = sizeof(Object2);
	Object1* obj = new Object1();
	Object2* obj2 = new Object2();*/
#pragma endregion
	
}
