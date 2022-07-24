#pragma once
#include "pch.h"
template<typename T>
class Queue
{
private:
	class Node
	{
	public:
		Node(T _data)
		{
		/*	const char* type = typeid(T).name();
			bool check = false;
			char* ptr = _tcstok(const_cast<char*>(type), " ");
			while (ptr != NULL)
			{
				if (!strcmp(ptr, "char")||!strcmp(ptr,"const"))
				{
					ptr = _tcstok(NULL, " ");
				}
				else if(*ptr='[')
				{
					check = true;
				}
				else
				{
					break;
				}
			}
			if (check)
			{
				_tcscpy(static_cast<char*>(data), _data);
			}
			else
			{*/
				data = _data;
			//}
			next = nullptr;
			befor = nullptr;
		}
		Node* Next()
		{
			return next;
		}
		void Next(Node* _node)
		{
			next = _node;
		}
		Node* Befor()
		{
			return befor;
		}
		void Befor(Node* _node)
		{
			befor = _node;
		}
		const T const GetData()
		{
			return data;
		}
	private:
		T data;
		Node* next;
		Node* befor;
	};

	Node* start;
	Node* end;
	int size;
public:
	Queue()
	{
		start = nullptr;
		end = nullptr;
		size = 0;
	}
	void Push(T _data)
	{
		Node* node = new Node(_data);
		if (start == nullptr)
		{
			end = node;
			start = end;
		}
		else
		{
			end->Next(node);
			node->Befor(end);
			end = node;
		}
		size++;
	}
	void Pop()
	{
		if (start == nullptr)
		{
			_tprintf("%s", "큐가 비었습니다!");
			return;
		}

		Node* node = start;
		start = node->Next();
		delete node;
		size--;
	}
	T Front()
	{
		if (start == nullptr)
		{
			_tprintf("%s", "큐가 비었습니다!");
			return NULL;
		}
		return start->GetData();
	}
	T Back()
	{
		if (end == nullptr)
		{
			_tprintf("%s", "큐가 비었습니다!");
			return NULL;
		}
		return end->GetData();
	}
	size_t Size()
	{
		return size;
	}
	bool Empty()
	{
		if (size == 0)
		{
			return true;
		}
		else
			return false;
	}
	void Swap(Queue<T> _queue)
	{
		Queue<T>* temp;
		temp = this;
		this = &_queue;
		_queue = temp;
	}
};