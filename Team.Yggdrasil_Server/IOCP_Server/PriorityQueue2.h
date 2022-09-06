#pragma once
#include <list>
using namespace std;


template<typename T, typename Container = list<T>, typename _Pr = greater <typename Container::value_type>>
class PriorityQueue2
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
				if (sort(ptr->right->data, ptr->left->data))
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
	int capacity;
public:
	PriorityQueue2()
	{
		capacity = 45;
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
			{
				point = itr;
				break;
			}
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
		}
		contain.erase(point);*/
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



