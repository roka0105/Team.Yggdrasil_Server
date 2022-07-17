#pragma once
#include "pch.h"


#define baekjoonstrlen baekjoon_strlen
#define mystrlen my_strlen
#define mystrcpy_s my_strcpy_s
#define myprintf my_printf
#ifdef UNICODE
#define CODESIZE 2
#else
#define CODESIZE 1
#endif


void printfint(int _num)
{
	int count = 0;
	char str[BUFSIZE];
	memset(str, 0, BUFSIZE);

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

_Check_return_opt_
_CRT_STDIO_INLINE
unsigned int __CRTDECL my_strlen(const TCHAR* _str)
#if defined _NO_CRT_STDIO_INLINE
;
#else
{
	bool flag = false;
	int i = 0;
	while (1)
	{
		if (_str[i] == '\0')
			break;
		i++;
	}
	return i;
}
#endif


_Check_return_opt_
_CRT_STDIO_INLINE
TCHAR* __CRTDECL my_strcpy_s(TCHAR* _dest, const TCHAR* const _src, size_t _count)
#if defined _NO_CRT_STDIO_INLINE
;
#else
{
	TCHAR* temp = _dest;
	for (int i = 0; i < _count; i++)
	{
		_dest[i] = _src[i];
	}
	return temp;
}
#endif

_Check_return_opt_
_CRT_STDIO_INLINE
void __CRTDECL my_printf(const TCHAR* _Format, ...)
#if defined _NO_CRT_STDIO_INLINE
;
#else
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
#endif


_Check_return_opt_
_CRT_STDIO_INLINE
unsigned int __CRTDECL baekjoon_strlen(TCHAR* _str)
#if defined _NO_CRT_STDIO_INLINE
;
#else
{
	int count = 0;
	while (*_str)
	{
		switch (*_str)
		{
		case 'c':
			_str++;
			switch (*_str)
			{
			case '=':
			case '-':
				break;
			default:
				_str--;
				break;
			}
			break;
		case 'd':
			_str++;
			switch (*_str)
			{
			case '-':
				break;
			case 'z':
				_str++;
				if (*_str == '=')
				{
					break;
				}
				else
				{
					_str--;
				}
				_str--;
				break;
			default:
				_str--;
				break;
			}
			break;

		case 'l':
			_str++;
			if (*_str != 'j')
				_str--;
			break;
		case 'n':
			_str++;
			if (*_str != 'j')
				_str--;
			break;
		case 's':
			_str++;
			if (*_str != '=')
				_str--;
			break;
		case 'z':
			_str++;
			if (*_str != '=')
				_str--;
			break;
		}
		count++;
		_str++;
	}
	
	return count;
}
#endif

class String
{
public:
	String()
	{

	}
	String(const TCHAR* _str)
	{
		int strsize = mystrlen(_str);
		str = new TCHAR[strsize + 1];
		mystrcpy_s(str, _str, strsize);
		str[strsize] = '\0';
		m_strsize = strsize;
	}
	~String()
	{
		delete str;
	}
	TCHAR operator[](unsigned int num)
	{
		if (m_strsize < num)
		{
			_tprintf("잘못된 배열 접근\n");
			return NULL;
		}

		return str[num];
	}
	TCHAR* operator&()
	{
		return str;
	}
	const TCHAR* c_str()
	{
		return str;
	}
	size_t copy(TCHAR* const _str, size_t _len, size_t _offset)
	{
		str = new TCHAR[_len + 1];
		str[_len] = '\0';
		memcpy(str, _str + _offset, _len);
	}
private:
	TCHAR* str;
	unsigned int m_strsize;
};
list<String> strlist;
