#ifndef TIMER_H_
#define TIMER_H_

void timer_phase(int hz);
void timer_install();

extern unsigned int timer_ticks;
#define TICKS_SECOND 18

#endif /*TIMER_H_*/
