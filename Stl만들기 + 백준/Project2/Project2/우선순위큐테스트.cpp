#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <list>
#include <algorithm>
#include <functional>
#include "PriorityQueue.h"
using namespace std;

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
	bool operator>(const Person& _p)const
	{
		return this->id > _p.id;
	}
	bool operator<(const Person& _p)const
	{
		return this->id < _p.id;
	}
	bool operator>(const Person* _p)
	{
		return this->id > _p->id;
	}
	bool operator<(const Person* _p)
	{
		return this->id < _p->id;
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
		return a.GetID() > b.GetID();
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
	const std::function<bool(Person*, Person*)>& func = [](Person* first, Person* second)->bool {return first->GetID() > second->GetID(); };
	PriorityQueue < Person*, list<Person*>,func> queue;
	queue.Push(new Person(4, 2, 3));
	queue.Push(new Person(1, 2, 3));
	queue.Push(new Person(5, 2, 3));
	queue.Push(new Person(3, 2, 3));
	queue.Push(new Person(2, 2, 3));

	while (!queue.Empty())
	{
		cout << queue.Front()->GetID() << endl;
		queue.Pop();
	}

}