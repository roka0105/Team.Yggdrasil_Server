#include <iostream>
#include <vector>
using namespace std;

template <typename T>
bool test(T a) { return a == 1; }
template <typename T>
bool (*fp)(T a);
template <typename Arg,typename FuntionPointer = bool(*)(Arg)>
void test2(Arg a, FuntionPointer fp)
{
	(*fp)(a);
}
template <typename T>
void test3()
{

}
template <typename T>
class a
{
public :
	template<typename _func = bool(*)(T)>
	bool flag(_func f,T data)
	{
		return f(data);
	}
};
template <typename T,typename _func = bool(*)(T)>
class tee
{
public:
	_func f;
	bool flag(T data)
	{
		return f(data);
	}
};
template<typename _func = bool(*)(int)>
void A(_func a)
{

}
int main()
{
	//�̷��Դ� �Լ������Ͱ� �ȵ���. �ֳ�? �ڷ����� �ƴϰŵ�!
	tee<int, bool(*)(int)> a;
	tee<int> tt;
	tt.f = test;
	cout<<tt.f(1);

	
	fp<int> = test<int>;
	test2(1, test<int>);
	//a<int> aa;
	cout<<aa.flag(test,1)<<endl;
}