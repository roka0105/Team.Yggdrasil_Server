#pragma once
enum class NodeType
{
	Black,
	Red
};

class NilNode
{
private:

public:
	NilNode() :type(NodeType::Black) {}
	virtual ~NilNode() {}
	NodeType type;
};
static NilNode* nilnode = new NilNode();


template<typename T>
class Node :public NilNode
{
public:
	Node(T _data, NodeType _type) :parent(nullptr), left(nilnode), right(nilnode),data(_data)
	{
		type = _type;
	}
	~Node()
	{

	}
protected:
	Node* parent;
	NilNode* left;
	NilNode* right;
	T data;
};
template <typename T>
class Node<T*> :public NilNode
{
public:
	Node(T* _data, NodeType _type) :parent(nullptr), left(nilnode), right(nilnode), data(_data)
	{
		type = _type;
	}
	~Node()
	{
		delete data;
	}
	Node* parent;
	NilNode* left;
	NilNode* right;
	T* data;
};

template <typename T>
class Red_Black_Tree
{
private:
	NilNode* root;
#pragma region red_black_tree func
private:
	//특정 노드의 오른 or 왼쪽 노드가 닐인지 판단할 때.
	bool check_nillnode(NilNode* _node)
	{
		Node<T>* check = dynamic_cast<Node<T>*>(_node);
		return check == nullptr;
	}
	Node<T>** GetGrandParent(Node<T>* _node)
	{
		if (_node != nullptr)
		{
			return &(_node->parent->parent);
		}
		else
		{
			return nullptr;
		}
	}
	Node<T>** GetUncle(Node<T>* _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		if (_node != nullptr)
		{
			//부모가 할아버지의 왼쪽이면 오른쪽이 삼촌
			if (_node->parent == (*grand_parent)->left)
			{
				return &(*grand_parent)->right;
			}
			else
			{
				return &(*grand_parent)->left;
			}
		}
	}
	/* insert */
	//case 1 : 부모와 삼촌이 레드일 때 
	//  => 부모와 삼촌을 black 으로 바꾸고 root node가 red 인지 검사, red 면 black으로 변경.
	void insert_case1(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);

		(*_node)->type = NodeType::Black;
		(*uncle)->type = NodeType::Black;
		(*grand_parent)->type = NodeType::Red;

		RBT_Insert(&(*grand_parent));
	}
	//case 2 : 왼쪽에 push 됐을 때 부모의 오른쪽 push 그리고 부모가 레드 , 삼촌은 블랙 
	//  => 왼쪽으로 회전. 
	void insert_case2(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** parent = &(*_node)->parent;
		//본인 = black
		(*_node)->type = NodeType::Black;
		//부모 공간에 본인 넣기
		(*parent) = *_node;
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
		//부모에 기존 할아버지의 조상과 연결
		(*_node)->parent->parent = g_p_p;
		//부모에 오른쪽 기존 할아버지 연결 
		(*_node)->parent->right = (*grand_parent);
		(*grand_parent)/*할아버지 공간*/ = (*_node)->parent;
		RBT_Insert(&(*_node));
	}
	//case 4 : 오른쪽 push 됐을 때 부모의 왼쪽 push 그리고 부모가 레드 , 삼촌은 블랙
	void insert_case4(Node<T>** _node)
	{

	}
	//case 5 : 오른쪽 push 됐을 때 부모의 오른쪽 push 그리고 부모가 레드, 삼촌은 블랙 
	void insert_case5(Node<T>** _node)
	{

	}

	void RBT_Insert(Node<T>** _node)
	{
		
		if (_node == nullptr)
		{
			return;
			
		}//root node 일 경우
		else if (_node->parent == nullptr)
		{
			_node->type = NodeType::Black;
			return;
		}
		Node<T>* check = dynamic_cast<Node<T>*>((*_node));
		//node 가 아닌 nill node 인 경우 
		if (check == nullptr)
		{
			return;
		}

		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);
		if (_node->parent->type == NodeType::Red && (*uncle)->type == NodeType::Red)
		{
			insert_case1(_node);
		}
		else if (_node->parent->type == NodeType::Red && (*uncle)->type == NodeType::Black)
		{
			//왼쪽 부모에 push
			if (_node->parent == (*grand_parent)->left)
			{
				//부모의 오른쪽에 push
				if (_node->parent->right == _node)
					insert_case2(_node);
				//부모의 왼쪽에 push
				else
					insert_case3(_node);
			}
			//오른쪽 부모에 push
			else
			{
				if (_node->parent->left == _node)
					insert_case4(_node);
				else
					insert_case5(_node);
			}
		}
	}
#pragma endregion

#pragma region bst func
private:
	void push_node(NilNode** _root, Node<T>* _node)
	{
		Node<T>* cast = dynamic_cast<Node<T>*>(*_root);
		// 노드가 nil 이면 추가할 node 넣기.
		if (cast == nullptr)
		{
			*_root = _node;
		}
		else if (cast->data > _node->data)
		{
			_node->parent = cast;
			push_node(&(cast->left), _node);
		}
		else if (cast->data < _node->data)
		{
			_node->parent = cast;
			push_node(&(cast->left), _node);
		}
	}
	void child_up(NilNode** _root)
	{
		Node<T>* cast = dynamic_cast<Node<T>*>(*_root);
		if (cast == nullptr)
		{
			return;
		}
		if (cast->right == nullptr && cast->left == nullptr)
		{
			(*_root) = nullptr;
			return;
		}
		else if (cast->right == nullptr)
		{
			(*_root) = cast->left;
			//child_up(&((*_root)->left));
		}
		else if (cast->left == nullptr)
		{
			(*_root) = cast->right;
			//child_up(&((*_root)->right));
		}
		else
		{
			Node<T>* root_left;
			Node<T>* root_right;
			Node<T>* temp = (*_root);
			Node<T>* leftest = cast->right;
			while (leftest->left != nullptr)
			{
				temp = leftest;
				leftest = leftest->left;
			}

			if (temp->left == leftest)
			{
				root_left = cast->left;
				root_right = cast->right;
				(*_root) = leftest;
				/*Node* temp_right_node = new Node(leftest->right->data);
				temp->left = temp_right_node;
				temp_right_node->right = leftest->right->right;*/
				child_up(&(leftest));

				temp->left = leftest;

				cast->left = root_left;
				cast->right = root_right;
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
	void pop_node(NilNode** _root)
	{
		if (_root == nullptr)
			return;
		Node<T>* ptr = dynamic_cast<Node<T>*>(*_root);
		child_up(_root);
		delete ptr;
	}
	bool serch_node(NilNode** _root, T data, Node<T>**& serchnode)
	{
		Node<T>* cast = dynamic_cast<Node<T>*>(*_root);
		if (cast == nullptr)
			return false;
		if (cast->data == data)
		{
			serchnode = _root;
			return true;
		}
		else if (cast->data > data)
		{
			return serch_node(&(cast->left), data, serchnode);
		}
		else if (cast->data < data)
		{
			return serch_node(&(cast->right), data, serchnode);
		}
		return false;
	}
#pragma endregion
public:
	Red_Black_Tree() :root(nullptr)
	{

	}
	Node<T>* GetRoot()
	{
		return dynamic_cast<Node<T>*>(root);
	}
	void Push(T  data)
	{
		Node<T>* node = new Node<T>(data, NodeType::Red);
		push_node(&root, node);
		//RBT_Insert(&node);
	}
	bool Pop(T data)
	{
		Node<T>** serchnode;
		if (serch_node(&root, data, serchnode))
			pop_node(serchnode);

		return true;
	}

};

