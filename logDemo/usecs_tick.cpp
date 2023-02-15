#include "usecs_tick.h"

#include <windows.h>
#include <stdint.h>
int gettimeofday(struct timeval *tv, void* tz)
{
#define EPOCHFILETIME (116444736000000000ULL)
	FILETIME ft;
	LARGE_INTEGER li;
	uint64_t tt;

	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
	tv->tv_sec = tt / 1000000;
	tv->tv_usec = tt % 1000000;

	return 0;
}

int64_t CycleClock_Now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
}

int getmicroseconds()
{
	double now = CycleClock_Now() * 0.000001;;
	time_t timestamp_ = static_cast<time_t>(now);
	return static_cast<uint32_t>((now - timestamp_) * 1000000);
}