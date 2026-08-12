#ifndef TIMER_H
#define TIMER_H

#include "machine.h"

int get_mode_duration(WashMode mode);

void timer_tick(WashingMachine *machine);

void *timer_thread(void *arg);

#endif