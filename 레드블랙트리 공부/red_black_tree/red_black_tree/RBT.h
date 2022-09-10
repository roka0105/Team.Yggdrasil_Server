#pragma once

enum class NodeType
{
	None,
	Black,
	Red
};

template<typename T>
class Node
{
public:
	Node(NodeType _type) :parent(nullptr), type(_type), data(-1), is_extra(false)
	{

	}
	Node(T _data, Node<T>* nilnode, NodeType _type) :parent(nullptr), left(nilnode), right(nilnode), data(_data), type(_type), is_extra(false)
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
	bool is_extra;
};
template <typename T>
class Node<T*>
{
public:
	Node(NodeType _type) :parent(nullptr), type(_type), data(-1), is_extra(false)
	{

	}
	Node(T* _data, Node<T>* nilnode, NodeType _type) :parent(nullptr), left(nilnode), right(nilnode), data(_data), type(_type), is_extra(false)
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
	bool is_extra;
};
template <typename T>
class RBT
{
private:
	Node<T>* root;
	int count;
	static Node<T>* nilnode;
	//static Node<T>* extranode;
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
#pragma region insert func
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
		//Node<T>** parent = &(*_node)->parent;
		//���� = black
		//(*_node)->type = NodeType::Black;
		//�θ� ������ ���� �ֱ�
		//������ ���� ��� �ּҴ� �Ű���� �ϱ� ������ ��� ���α�
		//������ left = ���� �θ�
		//���� �θ� right = ��� ���� ��� �ּ�.
		//������ �������� ������ ��带 �������� �ٽ� �˻��ϱ�.
		Node<T>* curnode = (*_node);
		Node<T>** serch = nullptr;
		Node<T>* left_backup = curnode->left;
		curnode->left = nullptr;
		left_backup->parent = nullptr;
		serch_node(&root, curnode->parent->data, serch);
		Node<T>* temp = *serch;

		(*serch) = curnode;

		curnode->left = temp;
		curnode->parent = temp->parent;

		curnode->left->right = left_backup;
		curnode->left->parent = curnode;
		left_backup->parent = temp;


		RBT_Insert(&(*serch)->left);
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
		Node<T>** serch = nullptr;
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
		Node<T>* curnode = (*_node);
		Node<T>** serch = nullptr;
		Node<T>* right_backup = curnode->right;
		curnode->right = nullptr;
		right_backup->parent = nullptr;
		serch_node(&root, curnode->parent->data, serch);
		Node<T>* temp = *serch;

		(*serch) = curnode;

		curnode->right = temp;
		curnode->parent = temp->parent;

		curnode->right->left = right_backup;
		curnode->right->parent = curnode;
		right_backup->parent = temp;


		RBT_Insert(&(*serch)->left);
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
		temp_gpp->right = nilnode;
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

		if ((uncle != nullptr) && (*_node)->parent->type == NodeType::Red && (*uncle)->type == NodeType::Red)
		{
			insert_case1(_node);
		}
		else if ((uncle != nullptr) && (*_node)->parent->type == NodeType::Red && (*uncle)->type == NodeType::Black)
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
#pragma region delete func
#pragma region case 4
	/* case 4 ����
	:  doubly black (_node) �� ���� ��尡 Black �� ��
	   �� ������ ������ �ڳడ Red �̴�.

	   �ش�
	:  ������ ���� = �θ� ��
	   ������ ������ �ڳ�(Red) = Black
	   �θ� = Black
	   �������� �� : ���� ȸ�� (�θ� ����)
	   ������  ��  : ������ ȸ��(�θ����)
	*/
	//case 4 - r
	void delete_case4_r(Node<T>** _node)
	{

	}
	//case 4 - l
	void delete_case4_l(Node<T>** _node)
	{

	}
#pragma endregion
#pragma region case 3
	/* case 3 ����
	:  doubly black (_node) �� ���� ��尡 Black �� ��
	   �� ������ ���� �ڳడ Red �̴�.
	   �ش�
	:  ������ red�ڽ��� ���� �ٲ۴�.
	   ���� �������� ������ ���� ���鸸 ����/�� ������ ȸ��.
	   case 4 ����.
	*/
	//case 3 - r
	//case 3 - l

#pragma endregion
#pragma region case 2
	/* case 2 ����
	:  doubly black (_node)�� ���� ��尡 Black �� ��
	   ������ �ڳ�� ��� Black �̴�.

	   �ش�
	:  doubly black �� ���� ����� black �� ��Ƽ� �θ𿡰� ����.
	   �θ�� extra node ���°� �ȴ�.
	   �̶� �θ� red �̸� red-and-black ����
			�θ� black�̸� doubly black ���� �̴�.

	   �θ� root �̸� black ó���ϰ� ������
	   red-and-black �̸� black
	   doubly black �̸� �ٽ� case������ ���� �Ѵ�. RBT_Delete() ȣ��.
	*/
	//case 2

#pragma endregion
#pragma region case 1
	/* case 1 ����
	:  doubly black �� ������ Red �� ��

	   �ش�
	:  ������ Black ���� ����� RBT_Delete().
	*/
	//case 1 - r
	//case 1 - l
#pragma endregion
	void RBT_Delete(Node<T>** _node)
	{   
		Node<T>** parent = (*_node)->parent;
		Node<T>** sibling = nullptr;
		Node<T>* child_r = nullptr;
		Node<T>* child_l = nullptr;

		// ���� ��尡 �θ���� �����̴�.
		if ((*parent)->left == (*_node))
		{
			sibling = (*parent)->right;
			if ((*sibling)->type == NodeType::Black)
			{
				child_r = (*sibling)->right;
				child_l = (*sibling)->left;

				if (child_r->type == NodeType::Red)
				{

				}
				else
				{

				}
			}
			else // ������ red �� �� case 1 
			{

			}
		}
		else
		{
			sibling = (*parent)->left;
			if ((*sibling)->type == NodeType::Black)
			{

			}
			else // ������ red �� �� case 1
			{

			}
		}

		
	}
#pragma endregion
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
	void child_up(Node<T>** _root, NodeType& _delete_color)
	{
		Node<T>* ptr = (*_root);
		Node<T>* parent_ptr = (*_root)->parent;
		if (*_root == nilnode)
		{
			return;
		}
		//�ڽ��� ���� ���
		if (ptr->right == nilnode && ptr->left == nilnode)
		{
			_delete_color = ptr->type;
			(*_root) = nilnode;
		}//�ϳ��� �ִ� ���
		else if (ptr->right == nilnode)
		{
			_delete_color = ptr->type;
			(*_root) = ptr->left;
		}
		else if (ptr->left == nilnode)
		{
			_delete_color = ptr->type;
			(*_root) = ptr->right;
		}//�Ѵ� �ִ� ���
		else
		{
			NodeType use_color = NodeType::None;
			Node<T>* root_left;
			Node<T>* root_right;
			Node<T>* temp = (*_root);
			Node<T>* leftest = ptr->right;
			while (leftest->left != nilnode)
			{
				temp = leftest;
				leftest = leftest->left;
			}
			//������ �� : �����Ǵ� ����� successor ��� ��.
			_delete_color = leftest->type;
			//����� �� : �����Ǵ� ����� ��
			use_color = (*_root)->type;

			if (temp->left == leftest)
			{
				root_left = ptr->left;
				root_right = ptr->right;
				(*_root) = leftest;

				NodeType dummy_color = NodeType::None;
				child_up(&(leftest), dummy_color);

				temp->left = leftest;
				leftest->parent = temp;

				root_left->parent = (*_root);
				root_right->parent = (*_root);
				(*_root)->left = root_left;
				(*_root)->right = root_right;
			}
			else
			{
				Node<T>** tempnode = _root;
				root_left = temp->left;
				(*tempnode) = leftest;
				(*tempnode)->left = root_left;
				(*tempnode)->right = leftest->right;
				root_left->parent = (*tempnode);
			}
			//��ü�� successor ��忡 ������ ����� ������ ����.
			(*_root)->type = use_color;
		}
		//��ü�� successor ����� �θ� ����.
		(*_root)->parent = parent_ptr;
	}
	void pop_node(Node<T>** _root)
	{
		if (_root == nullptr)
			return;
		Node<T>* ptr = *_root;
		NodeType delete_color = NodeType::None;

		child_up(_root, delete_color);
		//���� �Ӽ��� �ִ��� üũ
		if (delete_color == NodeType::Black)
		{
			if ((*_root) == nilnode)
			{
				delete ptr;
				return;
			}
			else if((*_root)->parent==nullptr)
			{
				(*_root)->type = NodeType::Black;
				return;
			}
			// �����Ǵ� ���� black �̶�� ������ �ڸ��� ������ ���� successor ��忡 extra �Ӽ� �ο�.
			(*_root)->is_extra = true;

			//�̾� ���� successor�� extra �Ӽ��� �پ� �������̱� ������
			//�װ� �������� �Ӽ��� �����ϰ� ��� ���ġ.
			RBT_Delete(_root);
		}
		delete ptr;
	}

#pragma endregion
public:
	RBT() :root(nilnode)
	{

	}
	void Push(T  data)
	{
		Node<T>* node = new Node<T>(data, nilnode, NodeType::Red);
		push_node(&root, node);
		RBT_Insert(&node);
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
//template<typename T>
//Node<T>* RBT<T>::extranode = new Node<T>(NodeType::Black);