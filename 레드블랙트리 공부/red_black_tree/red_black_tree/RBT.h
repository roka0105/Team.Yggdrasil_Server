#pragma once

enum class NodeType
{
	Black,
	Red
};

template<typename T>
class Node 
{
public:
	Node(NodeType _type):parent(nullptr), type(_type),data(-1)
	{

	}
	Node(T _data, Node<T>* nilnode, NodeType _type) :parent(nullptr), left(nilnode), right(nilnode), data(_data),type(_type)
	{
	}
	~Node()
	{

	}
	Node* parent;
	Node* left;
	Node* right;
	T data;
	NodeType type;
};
template <typename T>
class Node<T*>
{
public:
	Node(NodeType _type) :parent(nullptr), type(_type), data(-1)
	{

	}
	Node(T* _data, Node<T>* nilnode, NodeType _type) :parent(nullptr), left(nilnode), right(nilnode), data(_data),type(_type)
	{
	}
	~Node()
	{
		delete data;
	}
	Node* parent;
	Node* left;
	Node* right;
	T* data;
	NodeType type;
};
template <typename T> 
class RBT
{
private:
	Node<T>* root;
	int count; 
	static Node<T>* nilnode;
private:
#pragma region red_black_tree func
private:
	bool serch_node(Node<T>** _root, T data, Node<T>**& serchnode)
	{
		Node<T>* ptr = *_root;
		if (*_root == nilnode)
			return false;
		if (ptr->data == data)
		{
			serchnode = _root;
			return true;
		}
		else if (ptr->data > data)
		{
			return serch_node(&((*_root)->left), data, serchnode);
		}
		else if (ptr->data < data)
		{
			return serch_node(&((*_root)->right), data, serchnode);
		}
		return false;
	}
	Node<T>** GetGrandParent(Node<T>** _node)
	{
		if ((*_node) != nilnode)
		{
			return &((*_node)->parent->parent);
		}
		else
		{
			return nullptr;
		}
	}
	Node<T>** GetUncle(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		if ((*grand_parent) == nullptr)
		{
			return nullptr;
		}
		if ((*_node) != nilnode)
		{
			//부모가 할아버지의 왼쪽이면 오른쪽이 삼촌
			if ((*_node)->parent == (*grand_parent)->left)
			{
				return &(*grand_parent)->right;
			}
			else
			{
				return &(*grand_parent)->left;
			}
		}
		return &nilnode;
	}
	/* insert */
	//case 1 : 부모와 삼촌이 레드일 때 
	//  => 부모와 삼촌을 black 으로 바꾸고 root node가 red 인지 검사, red 면 black으로 변경.
	void insert_case1(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);
		if ((*grand_parent) == nullptr)
		{
			return;
		}

		(*_node)->parent->type = NodeType::Black;
		(*uncle)->type = NodeType::Black;
		(*grand_parent)->type = NodeType::Red;

		RBT_Insert(&(*grand_parent));
	}
	//case 2 : 왼쪽에 push 됐을 때 부모의 오른쪽 push 그리고 부모가 레드 , 삼촌은 블랙 
	//  => 왼쪽으로 회전. 
	void insert_case2(Node<T>** _node)
	{
		Node<T>** parent = &(*_node)->parent;
		//본인 = black
		(*_node)->type = NodeType::Black;
		//부모 공간에 본인 넣기
		Node<T>** serch = nullptr;
		serch_node(&root, (*parent)->data, serch);
		(*serch) = *_node;
		//본인의 왼쪽 노드 주소는 옮겨줘야 하기 때문에 잠시 빼두기
		Node<T>* left_backup = (*_node)->left;
		//본인의 left = 이전 부모
		(*_node)->left = (*parent);
		//이전 부모 right = 잠깐 빼둔 노드 주소.
		(*parent)->right = left_backup;

		//본인의 왼쪽으로 설정된 노드를 기준으로 다시 검색하기.
		RBT_Insert(&(*_node)->left);
	}
	//case 3 : 왼쪽에 push 됐을 때 부모의 왼쪽 push 그리고 부모가 레드 , 삼촌은 블랙
	void insert_case3(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);

		
		//부모 = black 할아버지 = red 로 변경
		(*_node)->parent->type = NodeType::Black;
		(*grand_parent)->type = NodeType::Red;

		//할아버지의 위치에 부모를 넣고, 부모의 오른쪽에 할아버지가 들어가게 함.
		//이중포인터 작업 해줘야 하는데 일단 틀만 짜기.
		Node<T>* g_p_p = (*grand_parent)->parent;
		Node<T>* temp_gpp = (*_node)->parent->parent;
		temp_gpp->left = nilnode;
		Node<T>** serch=nullptr;
		serch_node(&root, temp_gpp->data, serch);
		//부모에 기존 할아버지의 조상과 연결
		(*_node)->parent->parent = g_p_p;
		//부모에 오른쪽 기존 할아버지 연결 
		Node<T>* right_backup = (*_node)->parent->right;
		temp_gpp->left = right_backup;
		right_backup->parent = temp_gpp;
		(*_node)->parent->right = temp_gpp;
		temp_gpp->parent = (*_node)->parent;
		 
		(*serch)/*할아버지 공간*/ = (*_node)->parent;
		RBT_Insert(&(*_node));
	}
	//case 4 : 오른쪽 push 됐을 때 부모의 왼쪽 push 그리고 부모가 레드 , 삼촌은 블랙
	void insert_case4(Node<T>** _node)
	{
		Node<T>** parent = &(*_node)->parent;
		//본인 = black
		(*_node)->type = NodeType::Black;
		//부모 공간에 본인 넣기
		Node<T>** serch = nullptr;
		serch_node(&root, (*parent)->data, serch);
		(*serch) = *_node;
		//본인의 오른쪽 노드 주소는 옮겨줘야 하기 때문에 잠시 빼두기
		Node<T>* right_backup = (*_node)->right;
		//본인의 left = 이전 부모
		(*_node)->left = (*parent);
		//이전 부모 right = 잠깐 빼둔 노드 주소.
		(*parent)->right = right_backup;

		//본인의 왼쪽으로 설정된 노드를 기준으로 다시 검색하기.
		RBT_Insert(&(*_node)->left);
	}
	//case 5 : 오른쪽 push 됐을 때 부모의 오른쪽 push 그리고 부모가 레드, 삼촌은 블랙 
	void insert_case5(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);


		//부모 = black 할아버지 = red 로 변경
		(*_node)->parent->type = NodeType::Black;
		(*grand_parent)->type = NodeType::Red;

		//할아버지의 위치에 부모를 넣고, 부모의 오른쪽에 할아버지가 들어가게 함.
		//이중포인터 작업 해줘야 하는데 일단 틀만 짜기.
		Node<T>* g_p_p = (*grand_parent)->parent;
		Node<T>* temp_gpp = (*_node)->parent->parent;
		temp_gpp->left = nilnode;
		Node<T>** serch = nullptr;
		serch_node(&root, temp_gpp->data, serch);
		//부모에 기존 할아버지의 조상과 연결
		(*_node)->parent->parent = g_p_p;
		//부모에 왼쪽 기존 할아버지 연결 
		Node<T>* left_backup = (*_node)->parent->left;
		temp_gpp->right = left_backup;
		left_backup->parent = temp_gpp;
		(*_node)->parent->left = temp_gpp;
		temp_gpp->parent = (*_node)->parent;
		(*serch)/*할아버지 공간*/ = (*_node)->parent;
		RBT_Insert(&(*_node));
	}

	void RBT_Insert(Node<T>** _node)
	{

		if ((*_node) == nilnode)
		{
			return;

		}//root node 일 경우
		else if ((*_node)->parent == nullptr)
		{
			(*_node)->type = NodeType::Black;
			return;
		}
	
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);
		if ((*_node)->parent->type == NodeType::Red && (*uncle)->type == NodeType::Red)
		{
			insert_case1(_node);
		}
		else if ((*_node)->parent->type == NodeType::Red && (*uncle)->type == NodeType::Black)
		{
			//왼쪽 부모에 push
			if ((*_node)->parent == (*grand_parent)->left)
			{
				//부모의 오른쪽에 push
				if ((*_node)->parent->right == (*_node))
					insert_case2(_node);
				//부모의 왼쪽에 push
				else
					insert_case3(_node);
			}
			//오른쪽 부모에 push
			else
			{
				if ((*_node)->parent->left == (*_node))
					insert_case4(_node);
				else
					insert_case5(_node);
			}
		}
	}
#pragma endregion
#pragma region BST func
	void push_node(Node<T>** _root, Node<T>* _node)
	{
		Node<T>* ptr = (*_root);
		// 노드가 nil 이면 추가할 node 넣기.
		if (*_root == nilnode)
		{
			*_root = _node;
		}
		else if (ptr->data > _node->data)
		{
			_node->parent = (*_root);
			push_node(&((*_root)->left), _node);
		}
		else if (ptr->data < _node->data)
		{
			_node->parent = (*_root);
			push_node(&((*_root)->right), _node);
		}
	}
	void child_up(Node<T>** _root)
	{
		Node<T>* ptr = (*_root);
		if (*_root == nilnode)
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
			(*_root) = ptr->left;
			//child_up(&((*_root)->left));
		}
		else if (ptr->left == nullptr)
		{
			(*_root) = ptr->right;
			//child_up(&((*_root)->right));
		}
		else
		{
			Node<T>* root_left;
			Node<T>* root_right;
			Node<T>* temp = (*_root);
			Node<T>* leftest = ptr->right;
			while (leftest->left != nullptr)
			{
				temp = leftest;
				leftest = leftest->left;
			}

			if (temp->left == leftest)
			{
				root_left = ptr->left;
				root_right = ptr->right;
				(*_root) = leftest;
				/*Node* temp_right_node = new Node(leftest->right->data);
				temp->left = temp_right_node;
				temp_right_node->right = leftest->right->right;*/
				child_up(&(leftest));

				temp->left = leftest;

				ptr->left = root_left;
				ptr->right = root_right;
			}
			else
			{
				Node<T>** tempnode = _root;
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
		Node<T>* ptr = *_root;
		child_up(_root);
		delete ptr;
	}
	
#pragma endregion
public:
	RBT():root(nilnode)
	{

	}
	void Push(T  data)
	{
		Node<T>* node = new Node<T>(data, nilnode, NodeType::Red);
		push_node(&root, node);
		if (count >= 2)
		{
			RBT_Insert(&node);
		}
		else
		{
			count++;
		}
	}
	bool Pop(T data)
	{
		Node<T>** serchnode;
		if (serch_node(&root, data, serchnode))
			pop_node(serchnode);

		return true;
	}

};
template<typename T>
Node<T>* RBT<T>::nilnode = new Node<T>(NodeType::Black);