#include <iostream>
using namespace std;
template <typename T>
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
			ptr->data = 0;
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
			if (_root->data > _root->left->data &&
				_root->left->data < _root->right->data)
			{
				swap(_root->data, _root->left->data);
				heapify(_root->left);
			}
			//부모노드값이 오른쪽 자식노드보다 큰데
			//오른쪽 자식 노드가 왼쪽 자식노드보다 작을때
			else if (_root->data > _root->right->data &&
				_root->right->data < _root->left->data)
			{
				swap(_root->data, _root->right->data);
				heapify(_root->right);
			}
		}
		else if (_root->left != nullptr)
		{
			if (_root->data > _root->left->data)
			{
				swap(_root->data, _root->left->data);
				heapify(_root->left);
			}
		}
		else if (_root->right != nullptr)
		{
			if (_root->data > _root->right->data)
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
};
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
			printf("%s", "큐가 비었습니다!");
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
			printf("%s", "큐가 비었습니다!");
			return NULL;
		}
		return start->GetData();
	}
	T Back()
	{
		if (end == nullptr)
		{
			printf("%s", "큐가 비었습니다!");
			return NULL;
		}
		return end->GetData();
	}
	void Insert(int _index, T _data)
	{
		if (start == nullptr)
			return;
		Node* curnode = start;
		for (int i = 0; i < size; i++)
		{
			if (_index == i)
			{
				Node* node = new Node(_data);
				node->Befor(curnode->Befor());
				node->Befor()->Next(node);
				node->Next(curnode);
				curnode->Befor(node);
				size++;
				break;
			}
			curnode = curnode->Next();
		}
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

	Queue<int> queue;
	queue.Push(1);
	queue.Push(3);
	queue.Push(4);
	queue.Push(6);
	queue.Insert(1, 2);
	queue.Insert(4, 5);


}