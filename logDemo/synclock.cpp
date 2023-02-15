
#include "synclock.h"

#if (defined(WIN16) || defined(WIN32) || defined(WIN64))

#include <windows.h>


Mutex::Mutex()
{
	m_critical_section = new _RTL_CRITICAL_SECTION;
	if(m_critical_section)
	{
		::InitializeCriticalSection(m_critical_section);
	}
}

Mutex::~Mutex()
{
	if(m_critical_section)
	{
		::DeleteCriticalSection(m_critical_section);
		delete m_critical_section;
		m_critical_section = NULL;
	}

}

void Mutex::Lock()
{
	if(m_critical_section)
	{
		::EnterCriticalSection(m_critical_section);
	}
}

void Mutex::Unlock()
{
	if(m_critical_section)
	{
		::LeaveCriticalSection(m_critical_section);
	}
}


#else

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

Mutex::Mutex()
{
	m_critical_section = new pthread_mutex_t;
	if(m_critical_section)
	{
		pthread_mutexattr_t attr;
		::pthread_mutexattr_init(&attr);
		::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		::pthread_mutex_init(m_critical_section, &attr);
		::pthread_mutexattr_destroy(&attr);
	}
}

Mutex::~Mutex()
{
	if(m_critical_section)
	{
		::pthread_mutex_destroy(m_critical_section);
		delete m_critical_section;
		m_critical_section = NULL;

	}

}

void Mutex::Lock()
{
	if(m_critical_section)
	{
		::pthread_mutex_lock(m_critical_section);
	}
}

void Mutex::Unlock()
{
	if(m_critical_section)
	{
		::pthread_mutex_unlock(m_critical_section);
	}
}


#endif