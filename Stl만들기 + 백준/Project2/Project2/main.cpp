#include <stdio.h>
#include <list>
#include <iostream>
using namespace std;
class A
{
public:
	virtual void Print()
	{
		printf("A\n");
	}
private:
	const int number = 1;
};
class C:public A
{
	virtual void  Print() override
	{
		A::Print();
		printf("B\n");
	}
private:
	int a;
};
class B:public A
{
public:
	virtual void  Print() override
	{
		A::Print();
		printf("B\n");
	}
private:
	int a;
};
int main()
{
	B* b=new B();
	C* c=new C();
	A* a;
	a = b;

	cout << typeid(b).name() << endl;
	cout << typeid(c).name() << endl;
	cout << typeid(B*).name() << endl;
	cout << typeid(a).name() << endl;
	cout << typeid(dynamic_cast<B*>(a)).name() << endl;
	if (typeid(dynamic_cast<B*>(a)) == typeid(B*))
		cout << "true"<<endl;
	if (typeid(dynamic_cast<C*>(a)) == typeid(B*))
		cout << "true2"<<endl;
	return 0;
}