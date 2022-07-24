#include "pch.h"
#include <iostream>
#include <stdarg.h>

#include <list>
#include <vector>
#include <queue>
#include <time.h>
#include <stdio.h>

using std::queue;
using std::list;
using std::vector;
#define BUFSIZE 256
void printfint(int _num)
{
	int count = 0;
	char str[BUFSIZE];

	for (int i = 0; i < BUFSIZE; i++)
		str[i] = '\0';

	if (_num < 0)
	{
		putc('-', stdout);
		_num = _num * (-1);
	}

	do
	{
		str[count++] = (_num % 10) + '0';
		_num = _num / 10;
	} while (_num);

	for (int i = count - 1; i >= 0; --i)
	{
		putc(str[i], stdout);
	}
}
void printfstring(char* _str)
{
	while (*_str)
	{
		putc(*_str++, stdout);
	}
}
void  my_printf(const char* _Format, ...)
{
	va_list ap;
	va_start(ap, _Format);
	int data;
	char* str;
	while (*_Format)
	{
		if (*_Format == '%')
		{
			_Format++;
			switch (*_Format)
			{
			case 'd':
				data = va_arg(ap, int);
				printfint(data);
				break;
			case 's':
				str = va_arg(ap, char*);
				printfstring(str);
				break;
			case 'c':
				data = va_arg(ap, int);
				putc(data, stdout);
				break;
			default:
				my_printf("%s", L"구현안한 형식지정자\n");
			}
		}
		else
		{
			putc(*_Format, stdout);
		}
		_Format++;
	}
	va_end(ap);
}
template<typename T>
class Queue
{
private:
	class Node
	{
	public:
		Node(T _data)
		{
			/*	const char* type = typeid(T).name();
				bool check = false;
				char* ptr = _tcstok(const_cast<char*>(type), " ");
				while (ptr != NULL)
				{
					if (!strcmp(ptr, "char")||!strcmp(ptr,"const"))
					{
						ptr = _tcstok(NULL, " ");
					}
					else if(*ptr='[')
					{
						check = true;
					}
					else
					{
						break;
					}
				}
				if (check)
				{
					_tcscpy(static_cast<char*>(data), _data);
				}
				else
				{*/
			data = _data;
			//}
			next = nullptr;
			befor = nullptr;
		}
		Node* Next()
		{
			return next;
		}
		void Next(Node* _node)
		{
			next = _node;
		}
		Node* Befor()
		{
			return befor;
		}
		void Befor(Node* _node)
		{
			befor = _node;
		}
		const T const GetData()
		{
			return data;
		}
	private:
		T data;
		Node* next;
		Node* befor;
	};

	Node* start;
	Node* end;
	int size;
public:
	Queue()
	{
		start = nullptr;
		end = nullptr;
		size = 0;
	}
	void Push(T _data)
	{
		Node* node = new Node(_data);
		if (start == nullptr)
		{
			end = node;
			start = end;
		}
		else
		{
			end->Next(node);
			node->Befor(end);
			end = node;
		}
		size++;
	}
	void Pop()
	{
		if (start == nullptr)
		{
			_tprintf("%s", "큐가 비었습니다!");
			return;
		}

		Node* node = start;
		start = node->Next();
		delete node;
		size--;
	}
	T Front()
	{
		if (start == nullptr)
		{
			_tprintf("%s", "큐가 비었습니다!");
			return NULL;
		}
		return start->GetData();
	}
	T Back()
	{
		if (end == nullptr)
		{
			_tprintf("%s", "큐가 비었습니다!");
			return NULL;
		}
		return end->GetData();
	}
	size_t Size()
	{
		return size;
	}
	bool Empty()
	{
		if (size == 0)
		{
			return true;
		}
		else
			return false;
	}
	void Swap(Queue<T> _queue)
	{
		Queue<T>* temp;
		temp = this;
		this = &_queue;
		_queue = temp;
	}
};
class Person
{
public:
	Person(int _id, int _time)
	{
		id = _id;
		time = _time;
	}
	Person(int _id, int _time, int _runtime)
	{
		id = _id;
		time = _time;
		runtime = _runtime;
	}
	int id;
	int time;
	int runtime;
};
vector<Person*> tempvec;

int main()
{
	clock_t start, end;
	
	list<Person*> wait_person;
	int open_befor_wait, max_time, end_time;
	_tscanf(_T("%d %d %d"), &open_befor_wait, &max_time, &end_time);
	for (int i = 0; i < open_befor_wait; i++)
	{
		int id = 0;
		int time = 0;
		_tscanf(_T("%d %d"), &id, &time);
		Person* person = new Person(id, time);
		wait_person.push_back(person);
	}
	int open_after_wait = 0;
	_tscanf(_T("%d"), &open_after_wait);
	vector<Person*> personvec;
	for (int i = 0; i < open_after_wait; i++)
	{
		int id, time, intime;
		_tscanf(_T("%d %d %d"), &id, &time, &intime);
		Person* person = new Person(id, time, intime);
		personvec.push_back(person);
	}
	start = clock();
	for (int i = 0; i < personvec.size() - 1; i++)
	{
		Person* min = personvec[i];
		for (int j = i + 1; j < personvec.size(); j++)
		{
			if (min->runtime > personvec[j]->runtime)
			{
				Person* temp = min;
				personvec[i] = personvec[j];
				personvec[j] = temp;
			}
		}
	}

	for (int i = 0; i < personvec.size(); i++)
		wait_person.push_back(personvec[i]);
	personvec.clear();

	Queue<int> idlog;

	int curtime = 0;
	while (wait_person.size() != 0)
	{

		Person* person = wait_person.front();
		wait_person.pop_front();
		int count = person->time > max_time ? max_time : person->time;
		for (int i = 0; i < count; i++)
		{
			idlog.Push(person->id);
			person->time--;
			curtime++;
		}
		if (person->time != 0)
		{
			person->runtime = curtime;
			for (std::list<Person*>::iterator itr = wait_person.begin(); itr != wait_person.end(); itr++)
			{
				if (person->runtime < (*itr)->runtime)
				{
					wait_person.insert(itr, person);
					break;
				}
			}
			wait_person.push_back(person);
		}

	}


	for (int i = 0; i < end_time; i++)
	{
		_tprintf("%d\n", idlog.Front());
		idlog.Pop();
	}
	end = clock();
	printf("%f", (float)(end - start));
}