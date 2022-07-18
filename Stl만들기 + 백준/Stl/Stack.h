#pragma once

template <typename T>
class Stack
{
private:
	class Node
	{
	public:
		Node(T _data)
		{
			data = _data;
		}
	
		T data;
		Node* next;
	};

	Node* top=nullptr;
public:
	T pop()
	{
		if (top == nullptr)
		{
			return NULL;
		}
		T data = top->data;
		top = top->next;
		return data;
	}
	void push(T _data)
	{
		Node* item = new Node(_data);
		if (top == nullptr)
			item->next = nullptr;
		else
		item->next = top;
		top = item;
	}
	T peek()
	{
		if (top == nullptr)
		{
			return NULL;
		}
		T data = top->data;
		return data;
	}
	bool IsEmpty()
	{
		if (top == nullptr)
			return true;
		else 
			return false;
	}
};