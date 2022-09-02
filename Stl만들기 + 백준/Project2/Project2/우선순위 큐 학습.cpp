#include <iostream>
#include <queue>
#include <functional>
#include <vector>
#include <list>
#include <stack>
#include <set>

using namespace std;
struct Student
{
	int id;
	int math, eng;
	Student(int num, int m, int e) : id(num), math(m), eng(e) {}
	bool operator>(const Student& b)
	{
		return this->id > b.id;
	}
	bool operator<(const Student& b)
	{
		return this->id < b.id;
	}
	/*bool operator<(const Student s)const
	{
		return this->id > s.id;
	}*/
};

struct cmp
{
	bool operator()(Student a, Student b)
	{
		return a.id > b.id;
	}
};
class a
{

};
int main()
{
	
	priority_queue<Student,vector<Student>,cmp> pq;
	
	pq.push(Student(3,100,50));
	pq.push(Student(1, 60, 50));
	pq.push(Student(2, 80, 50));
	pq.push(Student(4, 90, 50));
	pq.push(Student(5, 70, 40));

	cout << "우선순위 큐 사이즈 " << pq.size() << "\n";

	while (!pq.empty())
	{
		Student ts = pq.top(); pq.pop();
		cout << "(학번, 수학 , 영어 ) : " << ts.id << ' ' << ts.math << ' ' << ts.eng << '\n';
	}
	return 0;
}