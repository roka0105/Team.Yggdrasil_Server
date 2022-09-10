#pragma once

#define MAXMEMORY_BYTE 32768

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
#include <map>

using namespace std;

struct t_pool_info
{
	t_pool_info(const int _capacity, const int _size)
	{
		current = nullptr;
		char* next_ptr;
		char** ptr;
		char* ptr2;
		char* ptr3;

		current = (char*)malloc(_capacity);
		memset(current, 0, _capacity);

		next_ptr = (char*)current;
		startptr.push_back(current);
		
		for (int i = 1; i < _capacity/_size; i++)
		{
			ptr2 = current + (_size * i);
			ptr = &ptr2;
			memcpy(next_ptr, ptr, sizeof(char*));
			next_ptr += _size;
		}
		ptr2 = nullptr;
		ptr = &ptr2;
		// 맨 마지막에 nullptr 넣기 
		memcpy(next_ptr, ptr, sizeof(char*));

	}
	~t_pool_info()
	{
		int size = startptr.size();
		for (int i = 0; i < size; i++)
		{
			char* item = startptr.back();
			startptr.pop_back();
			free(item);
		}
	}
	char* current;
	//이미 생성된 블럭이 꽉 차서 새로운 블럭을 만들었을 때 그 블럭의 시작 주소.
	vector<char*> startptr;
	
};


class MemoryPool_2
{
public:
	static void* operator new(size_t _size);
	static void* operator new[](size_t size);

	static void operator delete(void* _object, size_t _size);

	static int AssignSize(size_t _size);
	static void End();

protected:
	static RBT</*객체크기*/UINT,/*pool 정보*/t_pool_info*> memorypools;
	
};


/*주의점 64bit 에서는 포인터가 8byte 이다.
  이 memory pool 의 경우 x byte 만큼 쪼개고 그 블럭의 맨 앞에 주소를 넣고 관리하다가
  new 로 내보낼때 해당 객체 타입으로 캐스팅을 하는것이기 때문에 
  만약 객체가 8byte 미만일 때 주소를 읽다가 짤리므로 터진다.*/