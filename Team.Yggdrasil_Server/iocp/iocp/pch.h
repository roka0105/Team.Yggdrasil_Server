#pragma once
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"libmysql.lib")

#include <Winsock2.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <list>
#include <queue>
#include <fstream>
using std::ofstream;
using std::ifstream;
using std::list;
using std::queue;

#include <mysql.h>

#include "func.h"
#include "define.h"

#define new new( _NORMAL_BLOCK, __FILE__,__LINE__)

