#pragma once 
template <typename T>
class Memorypool
{
public:
	static void* operator new(size_t _size);

	static void operator delete(void* _object);

	static void End();

protected:
	static const int count;
	static void* current;
	static void* startptr;
	static int capacity;
};
template<typename T>
const int Memorypool<T>::count = 50;
template<typename T>
void* Memorypool<T>::current = nullptr;
template<typename T>
void* Memorypool<T>::startptr = nullptr;
template<typename T>
int Memorypool<T>::capacity = 0;

template<typename T>
inline void* Memorypool<T>::operator new(size_t _size)
{
	T* next_ptr;
	if (current == nullptr)
	{
		current = malloc(_size * count);
		next_ptr = (T*)current;
		startptr = current;
		capacity += count;
		for (int i = 1; i < count; i++)
		{
			memcpy(next_ptr, (T*)(&current + (_size * i)), sizeof(T*));
			next_ptr += _size;
		}
		// 맨 마지막에 nullptr 넣기 
		memcpy(next_ptr, nullptr, sizeof(T*));
	}

	void* value = current;
	memcpy(&next_ptr, current, sizeof(T*));
	current = next_ptr;
	return value;
}

template<typename T>
inline void Memorypool<T>::operator delete(void* _object)
{
	memcpy(_object, &current, sizeof(T*));
	current = _object;
}

template<typename T>
inline void Memorypool<T>::End()
{
	while (capacity > 0)
	{
		current = startptr;
		free(startptr);
		startptr = current - (sizeof(T) * count);
		capacity -= count;
	}
}



