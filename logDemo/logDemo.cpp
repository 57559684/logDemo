// logDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "logger.h"
#include <iostream>

using namespace std;

#if 0

int _tmain(int argc, _TCHAR* argv[])
{

	char data[2]={0x0A,0x0B};
	for(int i=0; i<2; i++)
	{
		cout<<"result = "<< hex << int(data[i]) << " sp " <<dec << int(data[i])<< " sp " <<hex << int(data[i]) <<endl;
		cout<<"result = "<< int(data[i])<<endl;
	}

	char szExePath[MAX_PATH] = { 0 };
	char szDllPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szExePath, MAX_PATH*sizeof(char));
	char *temp = strrchr(szExePath, '\\');
	strncpy_s(szDllPath, szExePath, temp - szExePath + 1);
	strcat_s(szDllPath, MAX_PATH, "log");
	LOGGER::CLogger logger(LOGGER::Trace, szDllPath, "1test");
	LOGGER::CLogger *plogger = new LOGGER::CLogger(LOGGER::Trace, szDllPath, "2test");

	logger.SetFlushTime(1300);
	plogger->SetFlushTime(1400);

	ULONGLONG connt = 1;
	while(true)
	{
		pFATAL_LOG(&logger) << "TraceFatal " << connt;
		pERROR_LOG(&logger) << "TraceError " << "sun";
		pWARNING_LOG(&logger) << "中华人民共和国！";
		pINFO_LOG(&logger) << "TraceInfo";

		FATAL_LOG(logger) << "TraceFatal " << connt;
		ERROR_LOG(logger) << "TraceError " << "sun2";
		WARNING_LOG(logger) << "TraceWarning";
		INFO_LOG(logger) << "TraceInfo 这是一个中文测试";

		pFATAL_LOG(plogger) << "TraceFatal " << connt;
		pERROR_LOG(plogger) << "TraceError " << "sun";
		pWARNING_LOG(plogger) << "中华人民共和国！";
		pINFO_LOG(plogger) << "TraceInfo";

		FATAL_LOG(*plogger) << "TraceFatal " << connt;
		ERROR_LOG(*plogger) << "TraceError " << "sun2";
		WARNING_LOG(*plogger) << "TraceWarning";
		INFO_LOG(*plogger) << "TraceInfo 这是一个中文测试";

		pFATAL_LOG(&logger) <<"result = "<< hex << int(data[1]) << " sp " <<dec << int(data[1])<< " sp " <<hex << int(data[1]) << "\n";
		pFATAL_LOG(&logger) <<"result = "<< int(data[1])<< "\n";


		connt++;

		Sleep(1000);
	}

	if(plogger)
	{
		delete plogger;
		plogger = NULL;
	}
	
	return 0;
}

#else

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

#endif