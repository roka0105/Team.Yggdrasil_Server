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
	//Ư�� ����� ���� or ���� ��尡 ������ �Ǵ��� ��.
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
			//�θ� �Ҿƹ����� �����̸� �������� ����
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
	//case 1 : �θ�� ������ ������ �� 
	//  => �θ�� ������ black ���� �ٲٰ� root node�� red ���� �˻�, red �� black���� ����.
	void insert_case1(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);

		(*_node)->type = NodeType::Black;
		(*uncle)->type = NodeType::Black;
		(*grand_parent)->type = NodeType::Red;

		RBT_Insert(&(*grand_parent));
	}
	//case 2 : ���ʿ� push ���� �� �θ��� ������ push �׸��� �θ� ���� , ������ �� 
	//  => �������� ȸ��. 
	void insert_case2(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** parent = &(*_node)->parent;
		//���� = black
		(*_node)->type = NodeType::Black;
		//�θ� ������ ���� �ֱ�
		(*parent) = *_node;
		//������ ���� ��� �ּҴ� �Ű���� �ϱ� ������ ��� ���α�
		Node<T>* left_backup = (*_node)->left;
		//������ left = ���� �θ�
		(*_node)->left = (*parent);
		//���� �θ� right = ��� ���� ��� �ּ�.
		(*parent)->right = left_backup;

		//������ �������� ������ ��带 �������� �ٽ� �˻��ϱ�.
		RBT_Insert(&(*_node)->left);
	}
	//case 3 : ���ʿ� push ���� �� �θ��� ���� push �׸��� �θ� ���� , ������ ��
	void insert_case3(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);

	    //�θ� = black �Ҿƹ��� = red �� ����
		(*_node)->parent->type = NodeType::Black;
		(*grand_parent)->type = NodeType::Red;

		//�Ҿƹ����� ��ġ�� �θ� �ְ�, �θ��� �����ʿ� �Ҿƹ����� ���� ��.
		//���������� �۾� ����� �ϴµ� �ϴ� Ʋ�� ¥��.
		Node<T>* g_p_p = (*grand_parent)->parent;
		//�θ� ���� �Ҿƹ����� ����� ����
		(*_node)->parent->parent = g_p_p;
		//�θ� ������ ���� �Ҿƹ��� ���� 
		(*_node)->parent->right = (*grand_parent);
		(*grand_parent)/*�Ҿƹ��� ����*/ = (*_node)->parent;
		RBT_Insert(&(*_node));
	}
	//case 4 : ������ push ���� �� �θ��� ���� push �׸��� �θ� ���� , ������ ��
	void insert_case4(Node<T>** _node)
	{

	}
	//case 5 : ������ push ���� �� �θ��� ������ push �׸��� �θ� ����, ������ �� 
	void insert_case5(Node<T>** _node)
	{

	}

	void RBT_Insert(Node<T>** _node)
	{
		
		if (_node == nullptr)
		{
			return;
			
		}//root node �� ���
		else if (_node->parent == nullptr)
		{
			_node->type = NodeType::Black;
			return;
		}
		Node<T>* check = dynamic_cast<Node<T>*>((*_node));
		//node �� �ƴ� nill node �� ��� 
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
			//���� �θ� push
			if (_node->parent == (*grand_parent)->left)
			{
				//�θ��� �����ʿ� push
				if (_node->parent->right == _node)
					insert_case2(_node);
				//�θ��� ���ʿ� push
				else
					insert_case3(_node);
			}
			//������ �θ� push
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
		// ��尡 nil �̸� �߰��� node �ֱ�.
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

