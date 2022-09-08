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
			//�θ� �Ҿƹ����� �����̸� �������� ����
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
	//case 1 : �θ�� ������ ������ �� 
	//  => �θ�� ������ black ���� �ٲٰ� root node�� red ���� �˻�, red �� black���� ����.
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
	//case 2 : ���ʿ� push ���� �� �θ��� ������ push �׸��� �θ� ���� , ������ �� 
	//  => �������� ȸ��. 
	void insert_case2(Node<T>** _node)
	{
		Node<T>** parent = &(*_node)->parent;
		//���� = black
		(*_node)->type = NodeType::Black;
		//�θ� ������ ���� �ֱ�
		Node<T>** serch = nullptr;
		serch_node(&root, (*parent)->data, serch);
		(*serch) = *_node;
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
		Node<T>* temp_gpp = (*_node)->parent->parent;
		temp_gpp->left = nilnode;
		Node<T>** serch=nullptr;
		serch_node(&root, temp_gpp->data, serch);
		//�θ� ���� �Ҿƹ����� ����� ����
		(*_node)->parent->parent = g_p_p;
		//�θ� ������ ���� �Ҿƹ��� ���� 
		Node<T>* right_backup = (*_node)->parent->right;
		temp_gpp->left = right_backup;
		right_backup->parent = temp_gpp;
		(*_node)->parent->right = temp_gpp;
		temp_gpp->parent = (*_node)->parent;
		 
		(*serch)/*�Ҿƹ��� ����*/ = (*_node)->parent;
		RBT_Insert(&(*_node));
	}
	//case 4 : ������ push ���� �� �θ��� ���� push �׸��� �θ� ���� , ������ ��
	void insert_case4(Node<T>** _node)
	{
		Node<T>** parent = &(*_node)->parent;
		//���� = black
		(*_node)->type = NodeType::Black;
		//�θ� ������ ���� �ֱ�
		Node<T>** serch = nullptr;
		serch_node(&root, (*parent)->data, serch);
		(*serch) = *_node;
		//������ ������ ��� �ּҴ� �Ű���� �ϱ� ������ ��� ���α�
		Node<T>* right_backup = (*_node)->right;
		//������ left = ���� �θ�
		(*_node)->left = (*parent);
		//���� �θ� right = ��� ���� ��� �ּ�.
		(*parent)->right = right_backup;

		//������ �������� ������ ��带 �������� �ٽ� �˻��ϱ�.
		RBT_Insert(&(*_node)->left);
	}
	//case 5 : ������ push ���� �� �θ��� ������ push �׸��� �θ� ����, ������ �� 
	void insert_case5(Node<T>** _node)
	{
		Node<T>** grand_parent = GetGrandParent(_node);
		Node<T>** uncle = GetUncle(_node);


		//�θ� = black �Ҿƹ��� = red �� ����
		(*_node)->parent->type = NodeType::Black;
		(*grand_parent)->type = NodeType::Red;

		//�Ҿƹ����� ��ġ�� �θ� �ְ�, �θ��� �����ʿ� �Ҿƹ����� ���� ��.
		//���������� �۾� ����� �ϴµ� �ϴ� Ʋ�� ¥��.
		Node<T>* g_p_p = (*grand_parent)->parent;
		Node<T>* temp_gpp = (*_node)->parent->parent;
		temp_gpp->left = nilnode;
		Node<T>** serch = nullptr;
		serch_node(&root, temp_gpp->data, serch);
		//�θ� ���� �Ҿƹ����� ����� ����
		(*_node)->parent->parent = g_p_p;
		//�θ� ���� ���� �Ҿƹ��� ���� 
		Node<T>* left_backup = (*_node)->parent->left;
		temp_gpp->right = left_backup;
		left_backup->parent = temp_gpp;
		(*_node)->parent->left = temp_gpp;
		temp_gpp->parent = (*_node)->parent;
		(*serch)/*�Ҿƹ��� ����*/ = (*_node)->parent;
		RBT_Insert(&(*_node));
	}

	void RBT_Insert(Node<T>** _node)
	{

		if ((*_node) == nilnode)
		{
			return;

		}//root node �� ���
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
			//���� �θ� push
			if ((*_node)->parent == (*grand_parent)->left)
			{
				//�θ��� �����ʿ� push
				if ((*_node)->parent->right == (*_node))
					insert_case2(_node);
				//�θ��� ���ʿ� push
				else
					insert_case3(_node);
			}
			//������ �θ� push
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
		// ��尡 nil �̸� �߰��� node �ֱ�.
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