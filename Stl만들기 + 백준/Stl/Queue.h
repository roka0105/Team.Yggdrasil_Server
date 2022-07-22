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
			const char* type = typeid(T).name();
			bool check = false;
			char* ptr = _tcstok(type, " ");
			while (ptr != NULL)
			{
				if (!strcmp(ptr, "char")||!strcmp(ptr,"const"))
				{
					ptr = _tcstok(NULL, " ");
				}
				else if(*ptr="[")
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
				_tcscpy(data, _data);
			}
			else
			{
				data = _data;
			}
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
	private:
		T data;
		Node* next;
		Node* befor;
	};

	Node* start;
	Node* end;
public:
	void Push(T _data)
	{
		Node* node = new Node(_data);
		if (start == nullptr)
		{
			start = node;
		}
		else
		{
			start.
		}
	}
	void Pop();
	T Front();
	T Back();
	size_t Size();
	bool Empty();
	void Swap();
};