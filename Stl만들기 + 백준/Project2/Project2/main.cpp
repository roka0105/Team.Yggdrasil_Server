#include <stdio.h>
#include <list>
#include <iostream>
using namespace std;
//#include <iostream>
//using namespace std;
#include <vector>
//#include "property.h"
//using namespace Yeram;
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
class Percentage
{
public: 
	void setPercent(int _value)
	{
		value = _value;
	}
	int getPercent()
	{
		return value;
	}
	void test()
	{

	}
	__declspec(property(get = getPercent, put = setPercent)) int Percent;
private:
	int value;
};
//class Test
//{
//public:
//	friend Yeram::property<Test, int>;
//	Test()
//	{
//		myValue.__Initalize(this, &Test::Get, &Test::Set);
//	}
//	void Set(const int& _value)
//	{
//		value = _value;
//	}
//	const int& Get()
//	{
//		return value;
//	}
//	void operator =(const Test& _ref)
//	{
//		value = _ref.myValue;
//	}
//	Yeram::property<Test, int> myValue;
//	static bool createpropery;
//private:
//	int value;
//};
//bool Test::createpropery = false;

class node
{
public:
	node() {}
	~node()
	{
		int count = 0;
		while (m_children.size() != 0)
		{
			node* item = m_children.back();
			m_children.pop_back();
			cout << "부모:" << item->parent << endl;
			cout << "자식" << count++ << ":" << item << endl;
			delete item;
		}
	}
	void addnode(node* _node)
	{
		_node->parent = this;
		m_children.push_back(_node);
	}
	node* const getchild(int index)
	{
		int i = 0;
		for (vector<node*>::iterator itr = m_children.begin();
			itr!=m_children.end(); itr++)
		{
			if (i == index)
			{
				return *itr;
			}
			i++;
		}
		
	}
private:
	vector<node*> m_children;
	node* parent;
};
int main()
{
	/*Test test;
	test.myValue.Set_requester(&test);
    
	test.myValue = 20;
	int a = test.myValue;
	B* b=new B();
	C* c=new C();
	A* a;
	a = b;
	C cc;
	cout << typeid(b).name() << endl;
	cout << typeid(c).name() << endl;
	cout << typeid(B*).name() << endl;
	cout << typeid(a).name() << endl;
	cout << typeid(dynamic_cast<B*>(a)).name() << endl;
	if (typeid(dynamic_cast<B*>(a)) == typeid(B*))
		cout << "true"<<endl;
	if (typeid(dynamic_cast<C*>(a)) == typeid(B*))
		cout << "true2"<<endl;*/
	node* root = new node();
	root->addnode(new node());
	root->addnode(new node());
	root->addnode(new node());
	root->getchild(0)->addnode(new node());
	root->getchild(1)->addnode(new node());
	delete root;
	return 0;
}