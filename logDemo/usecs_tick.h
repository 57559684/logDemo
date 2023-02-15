#pragma once

#ifndef _USECS_TICK
#define _USECS_TICK


int gettimeofday(struct timeval *tv, void* tz);
int getmicroseconds();

#endif  /* _USECS_TICK */