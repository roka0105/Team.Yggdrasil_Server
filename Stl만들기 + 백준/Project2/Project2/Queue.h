#pragma once
template <typename T>
class Queue
{
	class Node
	{
		Node(T data)
		{
			left = null;
			right = null;
		}
		Node* left;
		Node* right;
		T data;
	};
	void push_node(Node* root,T data,int _tree,bool& find)
	{
		if (_tree == tree)
		{
			return;
		}
		else if (root->left == nullptr)
		{
			root->left = new Node(data);
			return;
		}
		else if(root->right ==nullptr)
		{
			root->right = new Node(data);
			return;
		}
		
		push_node(root->left,data,_tree++,find);
		if (!find)
		{
			push_node(root->right, data, _tree++, find);
			if (!find)
			{
				tree++;
				push_node(root, data, 0, false);
			}
		}
		
	}
public:
	Node* root;
	int tree;
	Queue()
	{
		root = nullptr;
	}
	
	void push(T data)
	{
		if (root == nullptr)
		{
			root = new Node(data);
		}
		else
		{
			if (root->left == nullptr)
				root->left = new Node(data);
			else if (root->right == nullptr)
				root->right = new Node(data);
		}
	}
	void pop()
	{

	}
	T front()
	{

	}
	T back()
	{

	}
	bool empty()
	{

	}
	size_t size()
	{

	}
};