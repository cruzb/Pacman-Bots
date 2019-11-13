#ifndef SLEEP_H
#define SLEEP_H

#include "mss_timer/mss_timer.h"

typedef void (*timer_int_t)();

void usleep(uint32_t usecs);
void nsleep(uint32_t nsecs);

void timer_set(uint32_t usecs, timer_int_t timer_int);
uint32_t timer_get();
int timer_done();

#endif
