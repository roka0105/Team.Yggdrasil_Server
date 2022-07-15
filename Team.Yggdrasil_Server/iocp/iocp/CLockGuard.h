#pragma once

template<typename T>
class CLockGuard
{
public:
	CLockGuard(T* a);
	~CLockGuard();
	
private:
	T* m_key;
};

template<typename T>
CLockGuard<T>::CLockGuard(T* a)
	: m_key(a)
{
	a->lock();
}

template<typename T>
CLockGuard<T>::~CLockGuard()
{
	m_key->unlock();
}




