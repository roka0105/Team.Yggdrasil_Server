//#include "BST_1.h"
#include "RBT.h"
#include <iostream>

#pragma region NillNode 상속, dynamic cast로 node 정보 구분하기 테스트 
/*
enum class Type
{
	HI
};
class A
{
public:
	A() :type(Type::HI) {}
	virtual ~A() {}
	Type type;
};
static A* nilA = new A();
template <typename T>
class B :public A
{
public:
	B(T data) :left(nilA), right(nilA) {}
	~B() {}
	A* left;
	A* right;
	T data;
};
template <typename T>
class B<T*> :public A
{
public:
	B(T* data) :left(nilA), right(nilA) {}
	~B() { delete data; }
	A* left;
	A* right;
	T* data;
};
*/
#pragma endregion
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//BST_1<int> tree;
#pragma region 기본 이진트리 테스트 
	/*tree.Push(7);
	tree.Push(3);
	tree.Push(11);
	tree.Push(1);
	tree.Push(5);
	tree.Push(9);
	tree.Push(13);
	tree.Push(0);
	tree.Push(2);
	tree.Push(4);
	tree.Push(6);
	tree.Push(8);
	tree.Push(10);
	tree.Push(12);
	tree.Push(14);

	bool flag=false;
	flag = tree.Pop(11);
	flag = tree.Pop(12);
	flag = tree.Pop(13);*/
#pragma endregion
#pragma region 기본 이진트리 테스트 2
#pragma endregion

#pragma region NillNode 상속, dynamic cast로 node 정보 구분하기 테스트 
/*
	B<int>* data=new B<int>(0);
	data->left = new B<int>(1);
	
	B<int>* cast = dynamic_cast<B<int>*>(data->left);
	if (cast == nullptr)
	{
		std::cout << "nill" << std::endl; // nill 인것
	}
	else
	{
		std::cout << "class b" << std::endl;
	}
	cast = dynamic_cast<B<int>*>(data->right);
	if (cast == nullptr)
	{
		std::cout << "nill" << std::endl; // nill 인것
	}
	else
	{
		std::cout << "class b" << std::endl;
	}
*/
#pragma endregion

#pragma region RBT Push Test
	RBT<int,char> tree;
	//test1

	/*
	tree.Push(11);
	tree.Push(2);
	tree.Push(14);
	tree.Push(1);
	tree.Push(7);
	tree.Push(15);
	tree.Push(5);
	tree.Push(8);
	tree.Push(4);*/

	//test2
	/*tree.Push(10);
	tree.Push(9);
	tree.Push(8);
	tree.Push(7);
	tree.Push(6);
	tree.Push(5);
	tree.Push(4);
	tree.Push(3);
	tree.Push(2);
	tree.Push(1);*/
	
	//test3

	/*tree.Push(1);
	tree.Push(2);
	tree.Push(3);
	tree.Push(4);
	tree.Push(5);
	tree.Push(6);
	tree.Push(7);
	tree.Push(8);
	tree.Push(9);
	tree.Push(10);*/
#pragma endregion

#pragma region  RBT Pop Test
	/*tree.Push(35);
	tree.Push(20);
	tree.Push(50);
	tree.Push(10);
	tree.Push(30);
	tree.Push(40);
	tree.Push(80);
	tree.Push(5);
	tree.Push(15);
	tree.Push(25);
	tree.Push(33);
	tree.Push(37);
	tree.Push(45);
	tree.Push(2);
	tree.Push(27);

	tree.Pop(15);
	tree.Pop(33);
	tree.Pop(37);
	tree.Pop(35);
	tree.Pop(40);
	tree.Pop(50);
	tree.Pop(80);
	tree.Pop(27);*/
#pragma endregion

#pragma region RBT Map Test
	tree.Push(1, 'A');
	tree.Push(2, 'B');
	tree.Push(3, 'C');
	tree.Push(4, 'D');
	tree.Push(5, 'E');
	tree.Push(6, 'F');
	tree.Push(7, 'G');
	tree.Push(8, 'H');


	int size = tree.Size();
	for (int i = 1; i <= size; i++)
	{
		std::cout << tree[i] << std::endl;
	}

	std::cout << "====================" << std::endl;
	tree.Pop(5);
	size = tree.Size();
	for (int i = 1; i <= size+1; i++)
	{
		if (i == 5)
		{
			continue;
		}
		std::cout << tree[i] << std::endl;
	}

	
#pragma endregion
}
