
#ifndef __SYNC_LOCK_H_
#define __SYNC_LOCK_H_

////////////////////////////////////////////////////////////////////////////////

#if (defined(WIN16) || defined(WIN32) || defined(WIN64))

struct _RTL_CRITICAL_SECTION;
class Mutex
{
public:
	Mutex();

	virtual ~Mutex();

	void Lock();

	void Unlock();

private:
	_RTL_CRITICAL_SECTION* m_critical_section;
};


#else

struct pthread_mutex_t;
class Mutex
{
public:
	Mutex();

	virtual ~Mutex();

	void Lock();

	void Unlock();

private:
	pthread_mutex_t* m_critical_section;
};

#endif


///////////////////////////////////////////////////////////
// ScopedLock

template <typename LOCK>
class MutexLock
{
public:
	MutexLock(LOCK &lock)
		: m_pLock(&lock)
	{
		m_pLock->Lock();
	}

	~MutexLock()
	{
		m_pLock->Unlock();
	}

private:
	LOCK *m_pLock;
};



#endif    //__SYNC_LOCK_H_