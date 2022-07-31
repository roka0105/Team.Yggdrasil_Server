#include <stdio.h>
class A
{
public:
	virtual void Print()
	{
		printf("A\n");
	}
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
	B b;

	b.Print();
	return 0;
}