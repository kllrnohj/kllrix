#include "timer.h"
#include "../Boot/irq.h"
#include "port.h"

unsigned int timer_ticks = 0;
unsigned int seconds = 0;
int _hz = 18;

unsigned int timer_seconds(){
	return seconds;
}

unsigned int get_timer_ticks(){
	return timer_ticks;
}

void timer_handler(struct regs *r){
	timer_ticks++; // we got another tick
	// 18 ticks = ~1 second woot (on default)
	if(timer_ticks % _hz == 0){
		seconds++;
		//kprint("One second has passed");
	}
}

void timer_install(){
	irq_install_handler(0, timer_handler);
}

void timer_phase(short hz){
	_hz = hz;
	short divisor = 1193180 / hz;
	out(0x43, 0x36); // set command byte to 0x36
	out(0x40, divisor & 0xFF); // set low byte divisor
	out(0x40, divisor >> 8); // set high byte of divisor
}
