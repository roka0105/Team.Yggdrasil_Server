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
	void push_node(Node* _root,T _data,int _tree,bool& _find)
	{
		
		if (_root->left == nullptr)
		{
			_root->left = new Node(_data);
			_find = true;
			return;
		}
		else if(_root->right == nullptr)
		{
			_root->right = new Node(_data);
			_find = true;
			return;
		}
		else if (_tree == tree)
		{
			return;
		}

		push_node(_root->left,data,_tree+1,_find);
		if (!_find)
		{
			push_node(_root->right,_data, _tree+1, _find);
			if (!_find)
			{
				tree++;
				push_node(_root, _data, 0, false);
			}
		}
		
	}
public:
	Node* root;
	int tree;
	int leaf;
	int find;
	Queue()
	{
		root = nullptr;
		tree = 0;
		leaf = 0;
		find
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
			push_node(root, data, 0, false);
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