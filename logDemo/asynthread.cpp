#include "asynthread.h"


#include <windows.h>
#include <process.h>

void __cdecl threadAddress(void *arg);

class ThreadData
{
public:
	ThreadFun func;
	ThreadData(const ThreadFun i_func):func(i_func){}
};

AsynThread::AsynThread()
{
}

AsynThread::~AsynThread()
{
}


 void __cdecl threadAddress( void *arg )
{
	 auto threadData = reinterpret_cast<ThreadData*>(arg);
	 threadData->func();
	 delete threadData;

    _endthread();
}

uintptr_t AsynThread::run(const ThreadFun &threadfun)
{
	ThreadData *arglist = new ThreadData(threadfun);
    if(NULL == arglist)
    {
        //qWarning() << __FUNCTION__ << " new alloc memory failed. " << ::GetLastError();
        return 0;
    }
	bool success = false;

	uintptr_t handle = -1;
	int tryCount = 5;
	do {
		handle = _beginthread(threadAddress, 0, (void*)arglist);
		success = (handle != -1);
		if (!success) {
			//qWarning() << __FUNCTION__ << "_beginthread fail:" << ::GetLastError();
			if (tryCount-- > 0) { //_beginthread failed ,Try again 5 times.
				::Sleep(1000);
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	} while (true);

	return handle;
}

