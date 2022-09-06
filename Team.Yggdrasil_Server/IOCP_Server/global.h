#pragma once
#ifndef _INCLUDE_GUARD_GLOBAL
#define _INCLUDE_GUARD_GLOBAL
enum class IO_TYPE
{
	NONE,
	ACCEPT,
	SEND,
	RECV,
	DISCONNECT,
};

struct t_ThreadInfo
{
	IO_TYPE io_type;
	E_STATE cur_state;
	unsigned long protocol;
};
map<DWORD, t_ThreadInfo*> g_threadinfo;
#endif