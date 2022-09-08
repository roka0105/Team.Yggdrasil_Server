#pragma once
#include <iostream>

template<typename T>
class Node
{
public:
	Node(T _data)
	{
		left = nullptr;
		right = nullptr;
		data = _data;
	}
	Node<T>* left;
	Node<T>* right;
	T data;
};
template <typename T>
class Node<T*>
{
public:
	Node(T* _data)
	{
		left = nullptr;
		right = nullptr;
		data = _data;
	}
	~Node()
	{
		delete data;
	}
	Node<T*>* left;
	Node<T*>* right;
	T* data;
};
template <typename T>
class BST_1
{
private:
	/*class Node
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
		NodeColor color;
	};*/
	Node<T>* root;
private:
	void push_node(Node<T>** _root, Node<T>* _node)
	{
		if (*_root == nullptr)
		{
			*_root = _node;
		}
		else if ((*_root)->data > _node->data)
		{
			push_node(&((*_root)->left), _node);
		}
		else if ((*_root)->data < _node->data)
		{
			push_node(&((*_root)->right), _node);
		}
	}
	void child_up(Node<T>** _root)
	{
		Node* ptr = (*_root);
		if (ptr == nullptr)
		{
			return;
		}
		if (ptr->right == nullptr && ptr->left == nullptr)
		{
			(*_root) = nullptr;
			return;
		}
		else if (ptr->right == nullptr)
		{
			(*_root) = (*_root)->left;
			//child_up(&((*_root)->left));
		}
		else if (ptr->left == nullptr)
		{
			(*_root) = (*_root)->right;
			//child_up(&((*_root)->right));
		}
		else
		{
			Node* root_left;
			Node* root_right;
			Node* temp = (*_root);
			Node* leftest = (*_root)->right;
			while (leftest->left != nullptr)
			{
				temp = leftest;
				leftest = leftest->left;
			}

			if (temp->left == leftest)
			{
				root_left = (*_root)->left;
				root_right = (*_root)->right;
				(*_root) = leftest;
				/*Node* temp_right_node = new Node(leftest->right->data);
				temp->left = temp_right_node;
				temp_right_node->right = leftest->right->right;*/
				child_up(&(leftest));

				temp->left = leftest;

				(*_root)->left = root_left;
				(*_root)->right = root_right;
			}
			else
			{
				Node** tempnode = _root;
				root_left = temp->left;
				(*tempnode) = leftest;
				(*tempnode)->left = root_left;
				(*tempnode)->right = leftest->right;

				//child_up(&(leftest->right));
			}
		}

	}
	void pop_node(Node<T>** _root)
	{
		if (_root == nullptr)
			return;
		Node* ptr = (*_root);
		/*
		Node* temp = ptr;
		*_root = nullptr;
		delete temp;
		*/
		child_up(_root);
		delete ptr;
	}
	bool serch_node(Node<T>** _root, T data, Node<T>**& serchnode)
	{
		if ((*_root) == nullptr)
			return false;
		if ((*_root)->data == data)
		{
			serchnode = _root;
			return true;
		}
		else if ((*_root)->data > data)
		{
			return serch_node(&(*_root)->left, data, serchnode);
		}
		else if ((*_root)->data < data)
		{
			return serch_node(&(*_root)->right, data, serchnode);
		}
		return false;
	}
public:
	BST_1() :root(nullptr)
	{

	}
	void Push(T  data)
	{
		Node<T>* node = new Node<T>(data);
		push_node(&root, node);
	}
	bool Pop(T data)
	{
		Node<T>** serchnode;
		if (serch_node(&root, data, serchnode))
			pop_node(serchnode);

		return true;
	}
};

