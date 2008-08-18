#ifndef __IRQ_H__
#define __IRQ_H__

#include "../Kernel/kernel.h"

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_install();
void irq_handler(struct regs* r);

#endif

