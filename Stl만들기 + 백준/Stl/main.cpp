#include "pch.h"
#include "String.h"
#include "Stack.h"

int main()
{
	Stack<char> chs;
	Stack<char> mark;
	char str[BUFSIZE];
	scanf("%s", str);
	char* ptr = str;
	int size = strlen(str);
	for (int i = size - 1; i >= 0; i--)
	{
		chs.push(ptr[i]);
	}
	char output[BUFSIZE];
	char* output_ptr = output;
	int check_count = 0;
	while (!chs.IsEmpty())
	{

		switch (chs.peek())
		{
		case '+':
		case '-':
		case '/':
		case '*':
		{	if (check_count > 0)
			mark.push(*ptr);
		    else if (*(++ptr) == '(')
		    {
			mark.push(*(--ptr));
		    }
		    else
		    {
		    	char temp = *ptr;
		    	ptr++;
		    	*output_ptr = *ptr;
		    	output_ptr++;
		    	*output_ptr = temp;
		    }
		break;
		}
		case '(':
			check_count++;
			break;
		case ')':
			check_count--;
			*output_ptr = mark.pop();
			output_ptr++;
			break;
		default:
			*output_ptr = chs.pop();
			output_ptr++;
			break;
		}
		ptr++;
	}


	my_printf("%s", output_ptr);

}