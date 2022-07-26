#include "pch.h"
#include <iostream>
#include <stdarg.h>

#include <list>
#include <vector>
#include <queue>
#include <time.h>
#include <stdio.h>

using std::greater;
using std::queue;
using std::list;
using std::vector;
#define BUFSIZE 256
void printfint(int _num)
{
	int count = 0;
	char str[BUFSIZE];

	for (int i = 0; i < BUFSIZE; i++)
		str[i] = '\0';

	if (_num < 0)
	{
		putc('-', stdout);
		_num = _num * (-1);
	}

	do
	{
		str[count++] = (_num % 10) + '0';
		_num = _num / 10;
	} while (_num);

	for (int i = count - 1; i >= 0; --i)
	{
		putc(str[i], stdout);
	}
}
void printfstring(char* _str)
{
	while (*_str)
	{
		putc(*_str++, stdout);
	}
}
void  my_printf(const char* _Format, ...)
{
	va_list ap;
	va_start(ap, _Format);
	int data;
	char* str;
	while (*_Format)
	{
		if (*_Format == '%')
		{
			_Format++;
			switch (*_Format)
			{
			case 'd':
				data = va_arg(ap, int);
				printfint(data);
				break;
			case 's':
				str = va_arg(ap, char*);
				printfstring(str);
				break;
			case 'c':
				data = va_arg(ap, int);
				putc(data, stdout);
				break;
			default:
				my_printf("%s", L"구현안한 형식지정자\n");
			}
		}
		else
		{
			putc(*_Format, stdout);
		}
		_Format++;
	}
	va_end(ap);
}

class Person
{
public:
	Person(int _id, int _time)
	{
		id = _id;
		time = _time;
	}
	Person(int _id, int _time, int _runtime)
	{
		id = _id;
		time = _time;
		runtime = _runtime;
	}
	bool operator>(const Person& _person)
	{
		return this->runtime > _person.runtime;
	}
	int id;
	int time;
	int runtime;
};
vector<Person*> tempvec;
struct PersonSort
{
	bool operator()(const Person& _person1, const Person& _person2) const
	{
		return _person1.runtime > _person2.runtime;
	}
	bool operator()(const Person* _person1, const Person* _person2) const
	{
		return _person1->runtime > _person2->runtime;
	}
};
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
				if (sort(_root->data, _root->right->data))
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

public:
	Queue()
	{

	}
	void Push(T _data)
	{
		contain.push_back(_data);
	}
	void Pop()
	{
		T data = contain.front();
		typename Container::iterator point;
		for (typename Container::iterator itr = contain.begin(); itr != contain.end(); itr++)
		{
			if (data == *itr)
			{
				point = itr;
				break;
			}
		}
		contain.erase(point);
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
int main()
{
	clock_t start, end;
	Queue<Person*, vector<Person*>, PersonSort> person_queue;
	
	int open_befor_wait, max_time, end_time;
	scanf("%d %d %d", &open_befor_wait, &max_time, &end_time);
	for (int i = 0; i < open_befor_wait; i++)
	{
		int id = 0;
		int time = 0;
		scanf("%d %d", &id, &time);
		Person* person = new Person(id, time,0);
		person_queue.Push(person);
	}
	int open_after_wait = 0;
	scanf("%d", &open_after_wait);
	vector<Person*> personvec;
	for (int i = 0; i < open_after_wait; i++)
	{
		int id, time, intime;
		scanf("%d %d %d", &id, &time, &intime);
		Person* person = new Person(id, time, intime);
		person_queue.Push(person);
	}
	start = clock();
	
	person_queue.Sort();
	Queue<int> idlog;
	int curtime = 0;
	while (person_queue.Size() != 0)
	{

		Person* person = person_queue.Front();
		person_queue.Pop();
		int count = person->time > max_time ? max_time : person->time;
		for (int i = 0; i < count; i++)
		{
			idlog.Push(person->id);
			person->time--;
			curtime++;
		}
		if (person->time != 0)
		{
			person->runtime = curtime;
			person_queue.Push(person);
			person_queue.Sort();
		}

	}


	for (int i = 0; i < end_time; i++)
	{
		_tprintf("%d\n", idlog.Front());
		idlog.Pop();
	}
	end = clock();
	printf("%f", (float)(end - start));
}