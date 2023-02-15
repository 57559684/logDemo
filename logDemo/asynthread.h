#ifndef  asynthread_INC
#define  asynthread_INC

#include <functional>
//using std::tr1::function;

typedef std::tr1::function<void()> ThreadFun;

class AsynThread
{
private:
	AsynThread();
    ~AsynThread();

public:
	static uintptr_t run(const ThreadFun &threadfun);

};

#endif   /* ----- #ifndef asynthread_INC  ----- */
