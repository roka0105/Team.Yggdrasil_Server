#pragma once
//#include "CLock.h"
class CLock;
template<typename T>
class CLock_Guard
{
public:
	CLock_Guard(T* a);
	~CLock_Guard();
	//CLock_Guard(T* a)
	//	: m_key(a)
	//{
	//	printf("����\n");
	//	a->lock();
	//}
	//~CLock_Guard()
	//{
	//	m_key->unlock();
	//}

private:
	T* m_key;

};
/* �̷������� ���� ��.
class Man
{

	CLock c;
	�Լ�A
	{
	LockGaurd a(c);
	}
};
*/
