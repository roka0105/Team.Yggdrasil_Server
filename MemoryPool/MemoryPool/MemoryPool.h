#pragma once // 어디서 사용함? main cpp 
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
using namespace std;

template <typename T>
class MemoryPool
{
public:
	static void* operator new(size_t _size);
	static void* operator new[](size_t size);

	static void operator delete(void* _object);

	static void End();

protected:
	static const int count;
	static char* current;
	static vector<char*> startptr;
	static int capacity;
};
template<typename T>
const int MemoryPool<T>::count = 2;
template<typename T>
char* MemoryPool<T>::current = nullptr;
template<typename T>
vector<char*> MemoryPool<T>::startptr;
template<typename T>
int MemoryPool<T>::capacity = 0;

template<typename T>
inline void* MemoryPool<T>::operator new(size_t _size)
{
	char* next_ptr;
	char** ptr;
	char* ptr2;
	char* ptr3;
	if (current == nullptr)
	{

		current = (char*)malloc(_size * count);
		memset(current, 0, _size * count);

		next_ptr = (char*)current;
		startptr.push_back(current);
		capacity += count;
		for (int i = 1; i < count; i++)
		{
			ptr2 = current + (_size * i);
			ptr = &ptr2;
			memcpy(next_ptr, ptr, sizeof(T*));
			next_ptr += _size;
		}
		ptr2 = nullptr;
		ptr = &ptr2;
		// 맨 마지막에 nullptr 넣기 
		memcpy(next_ptr, ptr, sizeof(T*));
	}

	char* value = current;
	memcpy(&next_ptr, current, sizeof(T*));
	current = next_ptr;
	return value;
}
//template<typename T>
//inline void* MemoryPool<T>::operator new[](size_t _size)
//{
//	char* next_ptr;
//	char** ptr;
//	char* ptr2;
//	char* ptr3;
//	if (current == nullptr)
//	{
//		current = (char*)malloc(sizeof(T) * (count + ((int)_size / sizeof(T))));
//		memset(current, 0, sizeof(T) * (count + ((int)_size / sizeof(T))));
//		startptr.push_back(current);
//		capacity += count;
//		
//		next_ptr = (char*)current;
//
//		for (int i = 1; i < count; i++)
//		{
//			ptr2 = current + (_size * i);
//			ptr = &ptr2;
//			memcpy(next_ptr, ptr, sizeof(T*));
//			next_ptr += _size;
//		}
//
//	}
//	char* value = current;
//	memcpy(&next_ptr, current + (sizeof(T) * ((int)_size / sizeof(T))), sizeof(T*));
//	current = next_ptr;
//	return value;
//}
template<typename T>
inline void MemoryPool<T>::operator delete(void* _object)
{
	memcpy((char*)_object, &current, sizeof(char*));
	current = (char*)_object;
}

template<typename T>
inline void MemoryPool<T>::End()
{
	int size = startptr.size();
	for (int i = 0; i < size; i++)
	{
		char* item = startptr.back();
		startptr.pop_back();
		free(item);
	}
}
