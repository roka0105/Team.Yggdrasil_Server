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
	//이렇게는 함수포인터가 안들어간다. 왜냐? 자료형이 아니거든!
	tee<int, bool(*)(int)> a;
	tee<int> tt;
	tt.f = test;
	cout<<tt.f(1);

	
	fp<int> = test<int>;
	test2(1, test<int>);
	//a<int> aa;
	cout<<aa.flag(test,1)<<endl;
}