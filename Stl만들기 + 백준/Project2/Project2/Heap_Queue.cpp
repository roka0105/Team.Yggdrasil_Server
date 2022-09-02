#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <list>
using namespace std;

template<typename T, typename Container = vector<T>, typename _Pr = greater <typename Container::value_type>>
class Queue
{
private:
	class Heap
	{
		class Node
		{
		public:
			Node(T _data)
			{
				left = nullptr;
				right = nullptr;
				data = _data;
			}
			Node* left;
			Node* right;
			T data;
		};
		int tree;
		int leaf;
		bool find;
		Node* root;
		Node* lastleaf;
		_Pr sort;
		void push_node(Node* _root, T _data, int _tree, bool& _find)
		{
			if (_root->left == nullptr)
			{
				_root->left = new Node(_data);
				_find = true;
				heapify(_root);
				return;
			}
			else if (_root->right == nullptr)
			{
				_root->right = new Node(_data);
				_find = true;
				heapify(_root);
				return;
			}
			if (_tree == tree)
			{
				return;
			}

			push_node(_root->left, _data, _tree + 1, _find);
			if (!_find)
			{
				push_node(_root->right, _data, _tree + 1, _find);
			}

		}
		T front_data()
		{
			return root->data;
		}
		void pop_node(Node** _root)
		{
			Node* ptr = (*_root);
			if (ptr->left != nullptr && ptr->right != nullptr)
			{
				if (ptr->left->data < ptr->right->data)
				{
					ptr->data = ptr->left->data;
					pop_node(&ptr->left);
				}
				else
				{
					ptr->data = ptr->right->data;
					pop_node(&ptr->right);
				}
			}
			else if (ptr->left == nullptr && ptr->right == nullptr)
			{
				Node* temp = ptr;
				*_root = nullptr;
				delete temp;
			}
			else if (ptr->left != nullptr)
			{
				ptr->data = ptr->left->data;
				pop_node(&ptr->left);
			}
			else if (ptr->right != nullptr)
			{
				ptr->data = ptr->right->data;
				pop_node(&ptr->right);
			}

		}
		void swap(T& _data, T& _data2)
		{
			T temp = _data;
			_data = _data2;
			_data2 = temp;
		}

		void heapify(Node* _root)
		{
			//부모노드값이 왼쪽 자식노드보다 큰데 
			//왼쪽 자식 노드가 오른쪽 자식노드보다 작을때
			if (_root->left != nullptr && _root->right != nullptr)
			{
				if (sort(_root->data, _root->left->data) &&
					sort(_root->right->data, _root->left->data))
				{
					swap(_root->data, _root->left->data);
						heapify(_root->left);
				}
				//부모노드값이 오른쪽 자식노드보다 큰데
				//오른쪽 자식 노드가 왼쪽 자식노드보다 작을때
				else if (sort(_root->data, _root->right->data) &&
					sort(_root->left->data, _root->right->data))
				{
					swap(_root->data, _root->right->data);
						heapify(_root->right);
				}
			}
			else if (_root->left != nullptr)
			{
				if (sort(_root->data, _root->left->data))
				{
					swap(_root->data, _root->left->data);
					heapify(_root->left);
				}
			}
			else if (_root->right != nullptr)
			{
				if (sort(_root->data , _root->right->data))
				{
					swap(_root->data, _root->right->data);
					heapify(_root->right);
				}
			}

		}
		void node_sort(Node* _root, int _tree)
		{
			if (_tree = tree)
				return;
			if (_root = nullptr)
				return;
		}
	public:
		Heap()
		{
			root = nullptr;
			tree = 0;
			find = false;
		}
		void push(T data)
		{
			if (root == nullptr)
			{
				root = new Node(data);
				tree = 1;
			}
			else
			{
				find = false;
				push_node(root, data, 0, find);
				if (!find)
				{
					tree++;
					push_node(root, data, 0, find);
				}
			}
		}
		T pop()
		{
			T data = front_data();
			pop_node(&root);
			return data;
		}
		bool empty()
		{
			return root == nullptr;
		}
	};
	Heap heap;
	Container contain;
	int capacity;
public:
	Queue()
	{
		capacity = 3;
	}
	void Push(T _data)
	{   
		if (contain.size() == capacity)
		{
			contain.pop_front();
		}
		contain.push_back(_data);
		Sort();
	}
	void Pop()
	{
		T data = contain.front();
		/*typename Container::iterator point;
		for (typename Container::iterator itr = contain.begin(); itr != contain.end(); itr++)
		{
			if (data == *itr)
				point = itr;
		}*/
		contain.remove(data);
	}
	void Sort()
	{
		for (T data : contain)
		{
			heap.push(data);
		}
		contain.clear();
		while (!heap.empty())
		{
			contain.push_back(heap.pop());
		}
	}
	T Front()
	{
		T data = contain.front();
		/*typename Container::iterator point;
		for (typename Container::iterator itr = contain.begin(); itr != contain.end(); itr++)
		{
			if (data == *itr)
			{
				point = itr;
				break;
			}
		}*/
		//contain.remove(data);
		return data;
	}
	size_t Size()
	{
		return contain.size();
	}
	bool Empty()
	{
		return contain.size() == 0;
	}
};
class Person
{

	int id;
	int math, eng;
public:
	Person() {}
	Person(int num, int m, int e) :id(num), math(m), eng(e) {}
	int GetID()
	{
		return id;
	}
	bool operator>(const Person& _p)const
	{
		return this->id > _p.id;
	}
	bool operator<(const Person& _p)const
	{
		return this->id < _p.id;
	}
	bool operator==(const Person& _p)const
	{
		return (this->id==_p.id&&this->eng==_p.eng&&this->math==_p.math);
	}
};

struct PersonSort
{
	bool operator()(Person a, Person b)
	{
		return a.GetID() > b.GetID();
	}
};

int main()
{
	
	/*int data[13];
	Heap<int> heap;
	heap.push(4);
	heap.push(3);
	heap.push(1);
	heap.push(2);
	heap.push(7);
	heap.push(8);
	heap.push(5);
	heap.push(6);
	heap.push(13);
	heap.push(11);
	heap.push(12);
	heap.push(10);
	heap.push(9);
	for (int i = 0; i < 13; i++)
		data[i] = heap.pop();*/

		/*Queue<int> queue;
		queue.Push(1);
		queue.Push(3);
		queue.Push(4);
		queue.Push(6);
		queue.Insert(1, 2);
		queue.Insert(4, 5);*/
	
	//Queue<Person, vector<Person>, PersonSort> queue;
	/*Queue<int, vector<int>, test> queue;

	Heap<Person> heap;
	heap.push(Person(4, 100, 100));
	heap.push(Person(2, 20, 20));
	heap.push(Person(1, 20, 10));
	heap.push(Person(5, 2, 2));
	heap.push(Person(8, 2, 2));
	heap.push(Person(6, 2, 2));
	heap.push(Person(7, 2, 2));
	heap.push(Person(15, 2, 2));
	heap.push(Person(10, 2, 2));
	heap.push(Person(11, 2, 2));
	heap.push(Person(13, 2, 2));
	heap.push(Person(12, 2, 2));
	heap.push(Person(14, 2, 2));

	for (int i = 0; i < 13; i++)
	{
		heap.pop().Print();
		cout << '\n';
	}*/

	/*
	정렬방식 안넣었을 때
	default 인 greater가 Person 의 operator > 를 호출해줌.
	*/
	
	PersonSort sort;
	Queue<Person,list<Person>,less<>> queue;
	queue.Push(Person(4, 2, 3));
	queue.Push(Person(1, 2, 3));
	queue.Push(Person(5, 2, 3));
	queue.Push(Person(3, 2, 3));
	queue.Push(Person(2, 2, 3));
	
	/*사용자 정의 operator 사용*/
	//Queue<Person, vector<Person>, PersonSort> queue;
	//queue.Push(Person(4, 2, 3));
	//queue.Push(Person(1, 2, 3));
	//queue.Push(Person(5, 2, 3));
	//queue.Push(Person(3, 2, 3));
	//queue.Push(Person(2, 2, 3));

	

	while (!queue.Empty())
	{
		cout << queue.Front().GetID() << endl;
		//queue.Pop();
	}

	/*vector<int> contain;
	contain.push_back(1);
	contain.push_back(2);
	contain.push_back(3);
	contain.push_back(4);
	contain.push_back(5);
	contain.push_back(6);
	vector<int>::iterator point;
	for (vector<int>::iterator itr = contain.begin(); itr != contain.end(); itr++)
	{
		if (4 == (*itr))
			point = itr;
	}
	contain.erase(point);*/

	
}