#ifndef __TIMER_H__
#define __TIMER_H__

void timer_install();
void timer_phase(short hz);
unsigned int timer_seconds();
unsigned int get_timer_ticks();

#endif

