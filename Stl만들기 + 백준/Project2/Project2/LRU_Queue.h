#pragma once
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

protected:
	PriorityQueue<T, _Pr>* queue;
private:
	int capacity;
};


