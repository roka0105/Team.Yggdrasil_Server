#include <iostream>
#include "LRU_Queue.h"

class Person
{
	int id;
	int math, eng;
public:
	Person() {}
	Person(int num, int m, int e) :id(num), math(m), eng(e) {}
	int GetID()
	{
		return id;
	}
	bool operator==(const Person& _p)const
	{
		return (this->id == _p.id && this->eng == _p.eng && this->math == _p.math);
	}
};

struct PersonSort
{
	bool operator()(Person a, Person b)
	{
		return a.GetID() < b.GetID();
	}
	bool operator()(Person* a, Person* b)
	{
		return a->GetID() > b->GetID();
	}
};

int main()
{
	/*vector<int> v;
	v.push_back(3);
	v.push_back(1);
	v.push_back(6);
	sort(v.begin(), v.end(), [](int first, int second)->bool{return first > second;});

	int count = v.size();
	for (int i = 0; i < count; i++)
	{
		cout<<v.front()<<endl;
		v.erase(v.begin());
	}*/
	LRU_Queue<Person*,PersonSort> queue(3);
	Person* data=nullptr;
	if (queue.Push(new Person(1, 2, 3), data))
		cout << data->GetID() << endl;
	data = nullptr;
	if(queue.Push(new Person(2, 2, 3),data))
		cout << data->GetID() << endl;
	data = nullptr;
	if(queue.Push(new Person(3, 2, 3),data))
		cout << data->GetID() << endl;
	data = nullptr;
	if(queue.Push(new Person(4, 2, 3),data))
		cout << data->GetID() << endl;
	data = nullptr;
	if(queue.Push(new Person(5, 2, 3),data))
		cout << data->GetID() << endl;
	data = nullptr;

	while (!queue.Empty())
	{
		cout << queue.Front()->GetID() << endl;
		queue.Pop();
	}
}