#include "pch.h"
#include <iostream>
#include <stdarg.h>
#include "Queue.h"
#include <list>
#include <vector>

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

class Person
{
public :
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
int main()
{
	Queue<Person*> wait_person;
	int open_befor_wait, max_time, end_time;
	_tscanf(_T("%d %d %d"), &open_befor_wait, &max_time, end_time);
	for (int i = 0; i < open_befor_wait; i++)
	{
		int id = 0;
		int time = 0;
		_tscanf(_T("%d %d"), &id, &time);
		Person* person = new Person(id, time);
		wait_person.Push(person);
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
	
	for (int i = 0; i < personvec.size()-1; i++)
	{
		Person* min = personvec[i];
		for (int j = i + 1; j < personvec.size(); j++)
		{
			if (min->time > personvec[j]->time)
			{
				Person* temp = min;
				min = personvec[i];
				personvec[i] = temp;
			}
		}
	}

	Queue<int> idlog;

	for (int i = 0; i < wait_person.Size(); i++)
	{
		Person* person = wait_person.Front();
		wait_person.Pop();
		for (int j = 0; j < max_time; j++)
		{
			idlog.Push(person->id);
			person->time--;
		}
		if (person->time != 0)
		{
			wait_person.Push(person);
		}
	}

	for (int i = 0; i < end_time; i++)
	{
		_tprintf("%d\n",idlog.Front());
		idlog.Pop();
	}
}