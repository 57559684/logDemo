# logDemo

```
CLogger Class,Win32

Class name: CLogger

Features:
Win platform log function
C++ standard library output format log
The log is written in a shared way (you can view the log when the program is running)
The logs of different modules can be written to different files
multi-thread safety
Support write log level setting
The log format includes log level, log time, file name, line number information, thread ID, function name
Support log buffer
Support log timing to write files
Support log instant write file
Support per log file
Support file compression and packaging


功能介绍：
Win平台日志功能
C++标准库输出格式日志
日志以共享方式写入（可以在程序运行时查看日志）
可以将不同模块的日志写到不同的文件中
多线程安全
支持写日志级别设置
日志格式包括日志级别、日志时间、文件名、行号信息、线程ID、函数名
支持日志缓冲区
支持日志定时写入文件
支持日志即时写入文件
支持每日志文件
支持文件压缩打包



作者：57559684 2023-1-10
如果您在使用本日志库时遇到问题，可以发邮件至<57559684@qq.com>

Usage:


#include "stdafx.h"
#include "logger.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	char szExePath[MAX_PATH] = { 0 };
	char szDllPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szExePath, MAX_PATH*sizeof(char));
	char *temp = strrchr(szExePath, '\\');
	strncpy_s(szDllPath, szExePath, temp - szExePath + 1);
	strcat_s(szDllPath, MAX_PATH, "log");
	LOGGER::CLogger logger(LOGGER::Trace, szDllPath, "3test");
	LOGGER::CLogger *plogger = new LOGGER::CLogger(LOGGER::Trace, szDllPath, "4test");

	logger.SetFlushTime(1200);
	plogger->SetFlushTime(1200);

	for(int i=0; i<100000; i++)
	{
		pFATAL_LOG(&logger) << "TraceFatal " << i;
		pERROR_LOG(&logger) << L"TraceError " << "sun";
		pWARNING_LOG(&logger) << "中华人民共和国！";
		pINFO_LOG(&logger) << "TraceInfo";

		FATAL_LOG(logger) << L"TraceFatal " << i;
		ERROR_LOG(logger) << "TraceError " << "sun2";
		WARNING_LOG(logger) << "TraceWarning";
		INFO_LOG(logger) << L"TraceInfo 这是一个中文测试";

		pFATAL_LOG(plogger) << "TraceFatal " << i;
		pERROR_LOG(plogger) << "TraceError " << "sun";
		pWARNING_LOG(plogger) << "中华人民共和国！";
		pINFO_LOG(plogger) << "TraceInfo";

		FATAL_LOG(*plogger) << "TraceFatal " << i;
		ERROR_LOG(*plogger) << "TraceError " << "sun2";
		WARNING_LOG(*plogger) << "TraceWarning";
		INFO_LOG(*plogger) << "TraceInfo 这是一个中文测试";

	}

	if(plogger)
	{
		delete plogger;
		plogger = NULL;
	}

	return 0;
}

running result：3test_2023-02-15.log file context:

[FATAL][2023-02-15 11:40:33.310389][logdemo.cpp:93][19176][wmain]TraceFatal 0
[ERROR][2023-02-15 11:40:33.310389][logdemo.cpp:94][19176][wmain]TraceError sun
[WARNING][2023-02-15 11:40:33.310389][logdemo.cpp:95][19176][wmain]中华人民共和国！
[INFO][2023-02-15 11:40:33.310389][logdemo.cpp:96][19176][wmain]TraceInfo
[FATAL][2023-02-15 11:40:33.310389][logdemo.cpp:98][19176][wmain]TraceFatal 0
[ERROR][2023-02-15 11:40:33.310389][logdemo.cpp:99][19176][wmain]TraceError sun2
[WARNING][2023-02-15 11:40:33.310389][logdemo.cpp:100][19176][wmain]TraceWarning
[INFO][2023-02-15 11:40:33.310389][logdemo.cpp:101][19176][wmain]TraceInfo 这是一个中文测试
[FATAL][2023-02-15 11:40:33.311359][logdemo.cpp:93][19176][wmain]TraceFatal 1
[ERROR][2023-02-15 11:40:33.311359][logdemo.cpp:94][19176][wmain]TraceError sun
[WARNING][2023-02-15 11:40:33.311359][logdemo.cpp:95][19176][wmain]中华人民共和国！
[INFO][2023-02-15 11:40:33.311359][logdemo.cpp:96][19176][wmain]TraceInfo
[FATAL][2023-02-15 11:40:33.311359][logdemo.cpp:98][19176][wmain]TraceFatal 1
[ERROR][2023-02-15 11:40:33.311359][logdemo.cpp:99][19176][wmain]TraceError sun2
[WARNING][2023-02-15 11:40:33.311359][logdemo.cpp:100][19176][wmain]TraceWarning
[INFO][2023-02-15 11:40:33.311359][logdemo.cpp:101][19176][wmain]TraceInfo 这是一个中文测试
[FATAL][2023-02-15 11:40:33.311359][logdemo.cpp:93][19176][wmain]TraceFatal 2
[ERROR][2023-02-15 11:40:33.311359][logdemo.cpp:94][19176][wmain]TraceError sun
[WARNING][2023-02-15 11:40:33.311359][logdemo.cpp:95][19176][wmain]中华人民共和国！
[INFO][2023-02-15 11:40:33.311359][logdemo.cpp:96][19176][wmain]TraceInfo
[FATAL][2023-02-15 11:40:33.312356][logdemo.cpp:98][19176][wmain]TraceFatal 2
[ERROR][2023-02-15 11:40:33.312356][logdemo.cpp:99][19176][wmain]TraceError sun2
[WARNING][2023-02-15 11:40:33.312356][logdemo.cpp:100][19176][wmain]TraceWarning
[INFO][2023-02-15 11:40:33.312356][logdemo.cpp:101][19176][wmain]TraceInfo 这是一个中文测试
```

