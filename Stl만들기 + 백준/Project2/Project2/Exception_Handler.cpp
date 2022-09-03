#include "Exception_Handler.h"
Exception_Handler* volatile Exception_Handler::m_instance = nullptr;
CLock* Exception_Handler::m_lock = new CLock();

DWORD Exception_Handler::_initalize(LPCTSTR dump_file_name, const MINIDUMP_TYPE dump_type)
{
    return 0;
}

DWORD Exception_Handler::run()
{
    return 0;
}

LONG __stdcall Exception_Handler::Exception_Callback(_EXCEPTION_POINTERS* exceptioninfo)
{
    return 0;
}
