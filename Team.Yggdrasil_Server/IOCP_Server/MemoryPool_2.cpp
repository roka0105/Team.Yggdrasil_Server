#include "pch.h"
#include "MemoryPool_2.h"

map<UINT, t_pool_info*> MemoryPool_2::memorypools;
const int MemoryPool_2::count = 100;

void* MemoryPool_2::operator new(size_t _size)
{
	char* next_ptr = nullptr;
	if (memorypools.find(_size) == memorypools.end())
	{
		//키 못찾음
		memorypools[_size] = new t_pool_info(count, _size);
	}
	else
	{
		//키 찾았는데 current 가 null 인 경우(이미 모두 할당한 경우 새로운 블럭 추가로 받기)
		if (memorypools[_size]->current == nullptr)
		{
			char** ptr;
			char* ptr2;
			memorypools[_size]->current = (char*)malloc(_size * count);
			memset(memorypools[_size]->current, 0, _size * count);

			next_ptr = (char*)memorypools[_size]->current;
			memorypools[_size]->startptr.push_back(memorypools[_size]->current);
			memorypools[_size]->capacity += count;
			for (int i = 1; i < count; i++)
			{
				ptr2 = memorypools[_size]->current + (_size * i);
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
	char* curptr = memorypools[_size]->current;
	char* result = curptr;
	

	memcpy(&next_ptr, curptr, sizeof(char*));
	memorypools[_size]->current = next_ptr;

	return result;
}

void MemoryPool_2::operator delete(void* _object, size_t _size)
{
	char* curptr = memorypools[_size]->current;
	memcpy((char*)_object, &curptr, sizeof(char*));
	memorypools[_size]->current = (char*)_object;
}

void MemoryPool_2::End()
{
	for (auto info : memorypools)
	{
		delete info.second;
	}
	memorypools.clear();
}
