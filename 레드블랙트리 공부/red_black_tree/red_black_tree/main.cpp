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
	RBT<int> tree;
	tree.Push(10);
	tree.Push(9);
	tree.Push(8);
	tree.Push(7);
	tree.Push(6);
	tree.Push(5);
	tree.Push(4);
	tree.Push(3);
	tree.Push(2);
	tree.Push(1);
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
	/*Red_Black_Tree<int> rbt;
	rbt.Push(11);
	rbt.Push(2);
	rbt.Push(4);
	rbt.Push(15);
	rbt.Push(1);
	rbt.Push(7);
	rbt.Push(5);
	rbt.Push(8);
	rbt.Push(4);*/
#pragma endregion
}
