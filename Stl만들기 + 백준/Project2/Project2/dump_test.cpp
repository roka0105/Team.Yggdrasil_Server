#include "Exception_Handler.h"

void create_exception(int* a = nullptr)
{
	a[100] = 100;
}
int main()
{
	Exception_Handler::Instance()->_initalize(L"handler.dmp");
	Exception_Handler::Instance()->run();
	create_exception();
}