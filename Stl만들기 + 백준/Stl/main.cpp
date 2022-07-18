#include <iostream>
#include <stdarg.h>

#define BUFSIZE 256

template <typename T>
class Stack
{
private:
	class Node
	{
	public:
		Node(T _data)
		{
			data = _data;
		}

		T data;
		Node* next;
	};

	Node* top = nullptr;
public:
	T pop()
	{
		if (top == nullptr)
		{
			return '\0';
		}
		T data = top->data;
		top = top->next;
		return data;
	}
	void push(T _data)
	{
		Node* item = new Node(_data);
		if (top == nullptr)
			item->next = nullptr;
		else
			item->next = top;
		top = item;
	}
	T peek()
	{
		if (top == nullptr)
		{
			return '\0';
		}
		T data = top->data;
		return data;
	}
	bool IsEmpty()
	{
		if (top == nullptr)
			return true;
		else
			return false;
	}
};
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

int main()
{
	char str[BUFSIZE];
	for (int i = 0; i < BUFSIZE; i++)
	{
		str[i] = '\0';
	}
	scanf("%s", str);

	Stack<char> op;
	char* ptr = str;
	while (*ptr)
	{
		switch (*ptr)
		{
		case '+':
		case '-':
			while (!op.IsEmpty()&&op.peek()!='(')
			{
					my_printf("%c", op.pop());
			}
			op.push(*ptr);
			break;
		case '*':
		case '/':
			if (!op.IsEmpty() && op.peek() == '*' || op.peek() == '/')
			{
				my_printf("%c", op.pop());
			}
			op.push(*ptr);
			break;
		case '(':
			op.push(*ptr);
			break;
		case ')':
			while (op.peek() != '(')
			{
				my_printf("%c", op.pop());
			}
			op.pop();
			break;
		default:
			my_printf("%c", *ptr);
		}
		ptr++;
	}
	while (!op.IsEmpty())
	{
		my_printf("%c", op.pop());
	}
	return 0;
}