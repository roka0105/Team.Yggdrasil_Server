#pragma once
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"libmysql.lib")

#include <Winsock2.h>
#include <stdio.h>
#ifdef UNICODE
#include <wchar.h>
#endif
#include <iostream>
#include <time.h>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <tchar.h>


using std::ofstream;
using std::ifstream;
using std::list;
using std::queue;
using std::vector;
using std::map;

#include <mysql.h>

#include "func.h"
#include "define.h"

//#include "CLock.h"
//#include "CLockGuard.h"
//#include "CLockGuard_Static.h"


#define new new( _NORMAL_BLOCK, __FILE__,__LINE__)
#define BUFSIZE 4096


#ifdef UNICODE
#define CODESIZE 2
#else
#define CODESIZE 1
#endif



