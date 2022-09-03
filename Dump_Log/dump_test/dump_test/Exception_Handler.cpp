#include "Exception_Handler.h"
#include "CLogMgr.h"
Exception_Handler* volatile Exception_Handler::m_instance = nullptr;
CLock* Exception_Handler::m_lock = new CLock();

DWORD Exception_Handler::_initalize(__in LPCTSTR dump_file_name, __in const MINIDUMP_TYPE dump_type)
{
    DWORD error = ERROR_SUCCESS;

    do {

        if (nullptr == dump_file_name) {
            error = ERROR_INVALID_PARAMETER;
            break;
        }

        _dump_file_name.assign(dump_file_name);
        _dump_type = dump_type;

    } while (false);

    CLogMgr::Create();
    CLogMgr::GetInst()->Init();
    return error;
}

DWORD Exception_Handler::run()
{
    _prev_filter = ::SetUnhandledExceptionFilter(Exception_Callback);
    return ERROR_SUCCESS;
}

LONG __stdcall Exception_Handler::Exception_Callback(__in _EXCEPTION_POINTERS* exceptioninfo)
{

    do {

        if (nullptr == exceptioninfo) {
            break;
        }

        SYSTEMTIME st = { 0 };
        ::GetLocalTime(&st);

        std::wstring dump_file_name;
        dump_file_name.assign(Exception_Handler::Instance()->_dump_file_name);

        //
        // create dump file.
        //

        HANDLE dump_file_handle = ::CreateFile(
            dump_file_name.c_str(),
            GENERIC_WRITE,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (INVALID_HANDLE_VALUE == dump_file_handle) {
            break;
        }

        MINIDUMP_EXCEPTION_INFORMATION ex_info = { 0 };

        ex_info.ThreadId = ::GetCurrentThreadId(); // Threae ID 설정
        ex_info.ExceptionPointers = exceptioninfo; // Exception 정보 설정
        ex_info.ClientPointers = FALSE;

        //
        // write dump file.
        //

        if (FALSE == ::MiniDumpWriteDump(
            ::GetCurrentProcess(),
            ::GetCurrentProcessId(),
            dump_file_handle,
            Exception_Handler::Instance()->_dump_type,
            &ex_info,
            nullptr, nullptr
        )) {

            break;
        }

    } while (false);

    MEMORYSTATUS memoryinfo;
    GlobalMemoryStatus(&memoryinfo);
  
    CLogMgr::GetInst()->
        FileWriteLog(_T("memory info\ndwMemoryLoad: %ld\ndwTotalPhys: %ld\ndwAvailPhys: %ld\ndwTotalPageFile: %ld\ndwAvilPageFile: %ld\ndwTotalVirtual: %ld\ndwAvailVirtual: %ld\n")
            , memoryinfo.dwMemoryLoad, memoryinfo.dwTotalPhys, memoryinfo.dwAvailPhys, memoryinfo.dwTotalPageFile, memoryinfo.dwAvailPageFile, memoryinfo.dwTotalVirtual, memoryinfo.dwAvailVirtual);
    
    return (Exception_Handler::Instance()->_prev_filter) ? Exception_Handler::Instance()->_prev_filter(exceptioninfo) : EXCEPTION_EXECUTE_HANDLER;

}
