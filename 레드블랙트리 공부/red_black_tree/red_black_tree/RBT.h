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
#pragma region insert func
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
		//Node<T>** parent = &(*_node)->parent;
		//본인 = black
		//(*_node)->type = NodeType::Black;
		//부모 공간에 본인 넣기
		//본인의 왼쪽 노드 주소는 옮겨줘야 하기 때문에 잠시 빼두기
		//본인의 left = 이전 부모
		//이전 부모 right = 잠깐 빼둔 노드 주소.
		//본인의 왼쪽으로 설정된 노드를 기준으로 다시 검색하기.
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
		Node<T>** serch = nullptr;
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
		temp_gpp->right = nilnode;
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

		if ((uncle != nullptr) && (*_node)->parent->type == NodeType::Red && (*uncle)->type == NodeType::Red)
		{
			insert_case1(_node);
		}
		else if ((uncle != nullptr) && (*_node)->parent->type == NodeType::Red && (*uncle)->type == NodeType::Black)
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
#pragma region delete func
#pragma region case 4
	/* case 4 설명
	:  doubly black (_node) 의 형제 노드가 Black 일 때
	   그 형제의 오른쪽 자녀가 Red 이다.

	   해답
	:  오른쪽 형제 = 부모 색
	   오른쪽 형제의 자녀(Red) = Black
	   부모 = Black
	   오른쪽일 시 : 왼쪽 회전 (부모 기준)
	   왼쪽일  시  : 오른쪽 회전(부모기준)
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
	/* case 3 설명
	:  doubly black (_node) 의 형제 노드가 Black 일 때
	   그 형제의 왼쪽 자녀가 Red 이다.
	   해답
	:  형제와 red자식의 색을 바꾼다.
	   형제 기준으로 형제에 속한 노드들만 오른/왼 쪽으로 회전.
	   case 4 수행.
	*/
	//case 3 - r
	//case 3 - l

#pragma endregion
#pragma region case 2
	/* case 2 설명
	:  doubly black (_node)의 형제 노드가 Black 일 때
	   형제의 자녀들 모두 Black 이다.

	   해답
	:  doubly black 과 형제 노드의 black 을 모아서 부모에게 전달.
	   부모는 extra node 상태가 된다.
	   이때 부모가 red 이면 red-and-black 상태
			부모가 black이면 doubly black 상태 이다.

	   부모가 root 이면 black 처리하고 나가고
	   red-and-black 이면 black
	   doubly black 이면 다시 case문들을 수행 한다. RBT_Delete() 호출.
	*/
	//case 2

#pragma endregion
#pragma region case 1
	/* case 1 설명
	:  doubly black 의 형제가 Red 일 때

	   해답
	:  형제를 Black 으로 만들고 RBT_Delete().
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

		// 현재 노드가 부모기준 왼쪽이다.
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
			else // 형제가 red 일 때 case 1 
			{

			}
		}
		else
		{
			sibling = (*parent)->left;
			if ((*sibling)->type == NodeType::Black)
			{

			}
			else // 형제가 red 일 때 case 1
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
	void child_up(Node<T>** _root, NodeType& _delete_color)
	{
		Node<T>* ptr = (*_root);
		Node<T>* parent_ptr = (*_root)->parent;
		if (*_root == nilnode)
		{
			return;
		}
		//자식이 없는 경우
		if (ptr->right == nilnode && ptr->left == nilnode)
		{
			_delete_color = ptr->type;
			(*_root) = nilnode;
		}//하나만 있는 경우
		else if (ptr->right == nilnode)
		{
			_delete_color = ptr->type;
			(*_root) = ptr->left;
		}
		else if (ptr->left == nilnode)
		{
			_delete_color = ptr->type;
			(*_root) = ptr->right;
		}//둘다 있는 경우
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
			//삭제할 색 : 삭제되는 노드의 successor 노드 색.
			_delete_color = leftest->type;
			//사용할 색 : 삭제되는 노드의 색
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
			//교체된 successor 노드에 색깔을 사용할 색으로 변경.
			(*_root)->type = use_color;
		}
		//교체된 successor 노드의 부모 설정.
		(*_root)->parent = parent_ptr;
	}
	void pop_node(Node<T>** _root)
	{
		if (_root == nullptr)
			return;
		Node<T>* ptr = *_root;
		NodeType delete_color = NodeType::None;

		child_up(_root, delete_color);
		//위반 속성이 있는지 체크
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
			// 삭제되는 색이 black 이라면 삭제된 자리에 가져다 놓은 successor 노드에 extra 속성 부여.
			(*_root)->is_extra = true;

			//이어 붙인 successor에 extra 속성이 붙어 있을것이기 때문에
			//그것 기준으로 속성에 부합하게 노드 재배치.
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