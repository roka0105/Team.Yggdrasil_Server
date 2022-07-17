#include "pch.h"
#include "String.h"
#include "Stack.h"

int main()
{
	Stack<char> chs;
	char str[BUFSIZE];
	scanf("%s", str);
	char* ptr = str;
	while (*ptr)
	{
		switch (*ptr)
		{
		case '(':
			break;
		case ')':
			break;

		}
		chs.push(*ptr);
		ptr++;
	}

	while (!chs.IsEmpty())
	{
		my_printf("%c", chs.pop());
	}
}