#include "PriorityQueue.h"
template<typename T,typename _Pr=greater<T>>
class LRU_Queue
{
public:
	LRU_Queue(int _capacity) :capacity(_capacity) { queue = new PriorityQueue<T,_Pr>(_capacity); }
	bool Push(T _data, T& outdata)
	{
		bool flag = false;
		if (queue->Size() == capacity)
		{
			outdata = queue->Front();
			queue->Pop();
			flag = true;
		}
		queue->Push(_data);
		return flag;
	}
	bool Empty()
	{
		return queue->Empty();
	}
	T Front()
	{
		return queue->Front();
	}
	void Pop()
	{
		queue->Pop();
	}
	int Size()
	{
		return queue->Size();
	}
	/*컨테이너의 값만 빼오는것이기 때문에 정렬 X,삭제 X 요소의 값들 출력만 한다.
	  lru에서 관리하는 render중인 타일에 대한 정보를 얻기 위해서 만들었다.*/
	const T& operator[](int _index)
	{
		return (*queue)[_index];
	}
protected:
	PriorityQueue<T, _Pr>* queue;
private:
	int capacity;
};
