// @Hustler's Project

#ifndef _ASM_INTERRUPT_H_
#define _ASM_INTERRUPT_H_

#include <asm/cpu.h>

void primary_gic_init(void);

// @Hustler [ISR]
void do_sync(void);
void do_irq(void);
void do_fiq(void);
void do_error(void);

#endif /* _ASM_INTERRUPT_H_ */
