#include "pch.h"
#include "MemoryPool_2.h"

RBT<UINT, t_pool_info*> MemoryPool_2::memorypools;


void* MemoryPool_2::operator new(size_t _size)
{
	char* next_ptr = nullptr;
	int size = AssignSize(_size);
	if (size == -1)
	{
		//뭔가 잘못 됨.
		return nullptr;
	}
	else if (size == 0)
	{
		// MAXMEMORY_BYTE/2 보다 값이 큰 경우 메모리풀을 생성하는게 낭비이기 때문에
		// 그 byte size 자체로 new 를 해준다.
		return malloc(_size);
	}
	t_pool_info* dummy=nullptr;
	if (memorypools.Find(size,dummy)==false)
	{
		//키 못찾음
		memorypools.Push(size, new t_pool_info(MAXMEMORY_BYTE, size));
	}
	else
	{
		//키 찾았는데 current 가 null 인 경우(이미 모두 할당한 경우 새로운 블럭 추가로 받기)
		if (memorypools[size]->current == nullptr)
		{
			char** ptr;
			char* ptr2;
			memorypools[size]->current = (char*)malloc(MAXMEMORY_BYTE);
			memset(memorypools[size]->current, 0, MAXMEMORY_BYTE);

			next_ptr = (char*)memorypools[size]->current;
			memorypools[size]->startptr.push_back(memorypools[size]->current);

			for (int i = 1; i < MAXMEMORY_BYTE / size; i++)
			{
				ptr2 = memorypools[size]->current + (size * i);
				ptr = &ptr2;
				memcpy(next_ptr, ptr, sizeof(char*));
				next_ptr += _size;
			}
			ptr2 = nullptr;
			ptr = &ptr2;
			// 맨 마지막에 nullptr 넣기 
			memcpy(next_ptr, ptr, sizeof(char*));
		}

	}
	char* curptr = memorypools[size]->current;
	char* result = curptr;


	memcpy(&next_ptr, curptr, sizeof(char*));
	memorypools[size]->current = next_ptr;

	return result;
}

void MemoryPool_2::operator delete(void* _object, size_t _size)
{
	int size = AssignSize(_size);
	if (size == -1)
	{
		//뭔가 잘못 됨.
		return;
	}
	else if (size == 0)
	{
		// MAXMEMORY_BYTE/2 보다 값이 큰 경우 메모리풀을 생성하는게 낭비이기 때문에
		// 그 byte size 자체로 new 를 했기 때문에 그 메모리 자체를 delete 해준다.
		free(_object);
		return;
	}
	char* curptr = memorypools[size]->current;
	memcpy((char*)_object, &curptr, sizeof(char*));
	memorypools[size]->current = (char*)_object;
}

int MemoryPool_2::AssignSize(size_t _size)
{
	if (_size * 2 > MAXMEMORY_BYTE)
		return 0;
	int befor = 0;
	for (int i = 8; i <= MAXMEMORY_BYTE / 2; i *=2)
	{
		if (_size > befor && _size <= i)
		{
			return i;
		}
		befor = i;
	}

	//음수인 경우
	return -1;
}

void MemoryPool_2::End()
{
	int size = memorypools.Size();
	for (int i=0;i<size;i++)
	{
		memorypools.Pop(i);
		//or
		//delete memorypools[i];
	}
}
