#include <vector>

using namespace std;


template<typename T, typename _Pr = greater <T>>
class PriorityQueue
{
protected:
	_Pr mycompare;
	vector<T> contain;
private:
	void VecPush(T _data)
	{
		contain.push_back(_data);

		for (int i = 1; i < contain.size(); i++)
		{
			int child = i;
			do
			{
				int root = (child - 1) / 2;
				if (mycompare(contain[root], contain[child]))
				{
					T temp = contain[root];
					contain[root] = contain[child];
					contain[child] = temp;
				}
				child = root;
			} while (child != 0);
		}
	}
	void VecSort()
	{
		if (contain.size() == 1)
		{
			return;
		}
		for (int i = contain.size() - 1; i >= 0; i--)
		{
			T temp = contain[0];
			contain[0] = contain[i];
			contain[i] = temp;
			int root = 0;
			int child = 1;
			do
			{
				child = root * 2 + 1;
				if (this->mycompare(contain[child], contain[child + 1]) && child < i - 1)
				{
					child++;
				}
				if (this->mycompare(contain[root], contain[child]) && child < i)
				{
					temp = contain[root];
					contain[root] = contain[child];
					contain[child] = temp;
				}
				root = child;
			} while (child < i);
		}
	}
public:
	PriorityQueue(int capacity=40)
	{
		contain.reserve(capacity);
	}
	void Push(T _data)
	{
		VecPush(_data);
		//VecSort();
	}
	void Pop()
	{
		int size = contain.size() - 1;
		T data = contain[size];

		for (auto itr = contain.begin(); itr != contain.end(); itr++)
		{
			if (data == *itr)
			{
				contain.erase(itr);
				break;
			}
		}

	}
	// 나중에 참조로 변경해야 함.
	T Front()
	{
		T temp = contain[0];
		int size = contain.size() - 1;
		contain[0] = contain[size];
		contain[size] = temp;
		int root = 0;
		int child = 1;
		do
		{
			child = root * 2 + 1;
			if (child < size - 1 && this->mycompare(contain[child], contain[child + 1]))
			{
				child++;
			}
			if (child < size && this->mycompare(contain[root], contain[child]))
			{
				temp = contain[root];
				contain[root] = contain[child];
				contain[child] = temp;
			}
			root = child;
		} while (child < size);

		T data = contain[size];
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
	const T& operator[](int _index)
	{
		return contain[_index];
	}
};





