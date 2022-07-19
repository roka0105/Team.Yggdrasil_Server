#pragma once
#include "pch.h"


#include <tchar.h>

namespace ProjectString
{
	void StringInit()
	{
#ifdef UNICODE
#pragma region Formatted I/O
		s_strlen = wcslen;
		s_printf = wprintf;
		s_fprintf = fwprintf;
		s_sprintf = swprintf;
		s_snprintf = _snwprintf;
		s_vprintf = vwprintf;
		s_vfprintf = vfwprintf;
		s_vsprintf = vswprintf;
		s_vsnprintf = _vsnwprintf;
		s_scanf = wscanf;
		s_fscanf = fwscanf;
		s_sscanf = swscanf;
		c_fgetc = fgetwc;
		c_fputc = fputwc;
		c_fgets = fgetws;
		c_fputs = fputws;
		c_getc = getwc;
		c_putc = putwc;
		c_getchar = getwchar;
		c_putchar = putwchar;
		c_gets_s = _getws_s;
		c_puts = _putws;
		f_fopen = _wfopen;

		s_itoa = _itow;
		s_ltoa = _ltow;
		s_ultoa = _ultow;
		s_atoi = _wtoi;
		s_atoll = _wtoll;

		s_strcat = wcscat;
		s_strchr = wcschr;
		s_strrchr = wcsrchr;
		s_strcmp = wcscmp;
		s_stricmp = _wcsicmp;
		s_strcpy = wcscpy;
		s_strstr = wcsstr;
		s_strtok = wcstok;
#pragma endregion
#else
#pragma region Formatted I/O
		s_strlen = strlen;
		s_printf = printf;
		s_fprintf = fprintf;
		s_sprintf = sprintf;
		s_snprintf = _snprintf;
		s_vprintf = vprintf;
		s_vfprintf = vfprintf;
		s_vsprintf = vsprintf;
		s_vsnprintf = _vsnprintf;
		s_scanf = scanf;
		s_fscanf = fscanf;
		s_sscanf = sscanf;
		c_fgetc = fgetc;
		c_fputc = fputc;
		c_fgets = fgets;
		c_fputs = fputs;
		c_getc = getc;
		c_putc = putc;
		c_getchar = getchar;
		c_putchar = putchar;
		c_gets_s = gets_s;
		c_puts = puts;
		f_fopen = fopen;

		s_itoa = _itoa;
		s_ltoa = _ltoa;
		s_ultoa = _ultoa;
		s_atoi = atoi;
		s_atoll = _atoll;

		s_strcat = strcat;
		s_strchr = strchr;
		s_strrchr = strrchr;
		s_strcmp = strcmp;
		s_stricmp = _stricmp;
		s_strcpy = strcpy;
		s_strstr = wcsstr;
		s_strtok = strtok;
#pragma endregion
#endif

	}

#ifdef UNICODE
	typedef unsigned short project_int_t;
	typedef wchar_t project_char_t;
#else
	typedef int project_int_t;
	typedef int project_char_t;
#endif

#pragma region Formatted I/O
	//out
    size_t(__cdecl* s_strlen)(const TCHAR* _String);
    int(__cdecl* s_printf)(const TCHAR* Format, ...);
    int(__cdecl* s_fprintf)(FILE* _Stream, const TCHAR* _Format, ...);
    int(*s_sprintf)(TCHAR* _Buffer, const TCHAR* _Format, ...);
    int(__cdecl* s_snprintf)(TCHAR* _Buffer, size_t _BufferCount, const TCHAR* _Format, ...);
    int(__cdecl* s_vprintf)(const TCHAR* _Format, va_list _Arglist);
    int(__cdecl* s_vfprintf)(FILE* _Stream, const TCHAR* _Format, va_list _Arglist);
    int(__cdecl* s_vsprintf)(TCHAR* _Buffer, const TCHAR* _Format, va_list _Arglist);
    int(__cdecl* s_vsnprintf)(TCHAR* _Buffer, size_t _BufferCount, const TCHAR* _Format, va_list _Arglist);
   
    project_int_t(__cdecl* c_fputc)(TCHAR _Character, FILE* _Stream);
    int(__cdecl* c_fputs)(const TCHAR* _Buffer, FILE* _Stream);
    project_int_t(__cdecl* c_putc)(project_char_t _Character, FILE* _Stream);
    project_int_t(__cdecl* c_putchar)(project_char_t _Character);
    int(__cdecl* c_puts)(const TCHAR* _Buffer);
	
	//in
    int(__cdecl* s_scanf)(const TCHAR* const _Format, ...);
    int(__cdecl* s_fscanf)(FILE* const _Stream, const TCHAR* _Format, ...);
    int(__cdecl* s_sscanf)(const TCHAR* _Buffer, const TCHAR* _Format, ...);
   
    project_int_t(__cdecl* c_fgetc)(FILE* _Stream);
    TCHAR* (__cdecl* c_fgets)(TCHAR* _Buffer, int _MaxCount, FILE* _Stream);
    project_int_t(__cdecl* c_getc)(FILE* _Stream);
    project_int_t(__cdecl* c_getchar)(void);
    TCHAR* (__cdecl* c_gets_s)(TCHAR* _Buffer, size_t sizeInCharacters);
#pragma endregion

#pragma region File Open
	FILE* (*f_fopen)(const TCHAR* _FileName, const TCHAR* _Mode);
#pragma endregion

#pragma region String Change
    TCHAR* (*s_itoa)(int _Val, TCHAR* _Buffer, int Radix);
    TCHAR* (*s_ltoa)(long _Val, TCHAR* _Buffer, int Radix);
    TCHAR* (*s_ultoa)(unsigned long _Val, TCHAR* _Buffer, int Radix);
    int(*s_atoi)(const TCHAR* _String);
    long long(*s_atoll)(const TCHAR* _String);
   
    TCHAR* (__cdecl* s_strcat)(TCHAR* _Destination, TCHAR const* _Source);
    TCHAR* (__cdecl* s_strchr)(TCHAR* const  _String, const project_char_t  _Ch);
    TCHAR* (__cdecl* s_strrchr)(TCHAR* const _String, const project_char_t _Ch);
    int(__cdecl* s_strcmp)(const TCHAR* _String1, const TCHAR* _String2);
    int(__cdecl* s_stricmp)(const TCHAR* _String1, const TCHAR* _String2);
    TCHAR* (__cdecl* s_strcpy)(TCHAR* _Destination, const TCHAR* _Source);
    TCHAR* (__cdecl* s_strstr)(TCHAR* const _String, const TCHAR* _Substr);
    TCHAR* (__cdecl* s_strtok)(TCHAR* _String, const TCHAR* _Delimiter);
#pragma endregion
	
}



