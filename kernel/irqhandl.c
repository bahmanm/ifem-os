/******************************************************************************
 * IFEM OS - kernel/irqhandle.c                                               *
 * Copyright (C) 2008 Bahman Movaqar (bahman AT bahmanm.com)                  *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,                 *
 * MA  02110-1301, USA.                                                       *
 ******************************************************************************/

/*
 * Primary interrupt handlers for i8259a IRQs. Most of these function do
 * nohting but checking and modifying some variables and then, if needed,
 * switching to appropriate task. This is needed due to the overhead of task-
 * switching. The IRQ handlers check the variables and if certain conditions
 * are met then switch to the task, otherwise they just return to the previous
 * process which was executing when interrupt occured.
 */
#include "kernel.h"

/* Master i8259a chip IRQs: */
PUBLIC int irq_00(int);        /* int 0x20, irq 0: clock */
PUBLIC int irq_01(int);        /* int 0x21, irq 1: keyboard */
PUBLIC int irq_02(int);        /* int 0x22, irq 2: cascade */
PUBLIC int irq_03(int);        /* int 0x23, irq 3: 2nd serial */
PUBLIC int irq_04(int);       /* int 0x24, irq 4: 1st serial */
PUBLIC int irq_05(int);       /* int 0x25, irq 5: XT winchester */
PUBLIC int irq_06(int);       /* int 0x26, irq 6: floppy */
PUBLIC int irq_07(int);       /* int 0x27, irq 7: printer */
/* Slave i8259a chip IRQs: */
PUBLIC int irq_08(int);       /* int 0x28, irq 8: real time clock */
PUBLIC int irq_09(int);       /* int 0x29, irq 9: irq2 redirected */
PUBLIC int irq_10(int);       /* int 0x2A, irq 10 */
PUBLIC int irq_11(int);       /* int 0x2B, irq 11 */
PUBLIC int irq_12(int);       /* int 0x2C, irq 12 */
PUBLIC int irq_13(int);       /* int 0x2D, irq 13: FPU exception */
PUBLIC int irq_14(int);       /* int 0x2E, irq 14: AT winchester */
PUBLIC int irq_15(int);       /* int 0x2F, irq 15 */

/*============================================================================*
 *				     irq_00				      *
 *============================================================================*/
PUBLIC int irq_00(int irq)
{
/* This is the IRQ handler for clock. It should count clock ticks, when enough
   call the clock task.
 */

  static unsigned long pit_ticks = 0;

  irq_status ^= (1 << IRQ_PIT);	 /* disable PIT IRQ line */
  _outb(PIC1_CTRL_MASK, irq_status);

  if (pit_ticks < TICKS2CALL)
  	pit_ticks++;
  else {
    change_attr(0x0b);
  	log_str("INT 0x20: PIT tick'ed 200 times.\n\0");
	pit_ticks =0;
  }

  irq_status ^= (1 << IRQ_PIT);	 /* enable PIT IRQ line */
  _outb(PIC1_CTRL_MASK, irq_status);
  _outb(PIC1_CTRL, PIC_ENABLE);		/* enable chips */

  return 1;	/* re-enable interrupts */
}

/*============================================================================*
 *				     irq_01				      *
 *============================================================================*/
PUBLIC int irq_01(int irq)
{
/* This routine is the preliminary IRQ handler one for keyboard interrupts.
   It should buffer KB_BUFFSIZE characters and then pass them to appropriate
   task.
 */

  #define KB_BUFFSIZE	32	/* size of buffer to store input before passing
  				   it to a task */
  #define KB_DATA	0x60	/* keyboard data I/O port */

  static unsigned char kb_buff[KB_BUFFSIZE];	/* should be global? */
  static short buff_items = 0;

  change_attr(0x0a);
  irq_status ^= (1 << IRQ_KB);	/* disable keyboard IRQ line */
  _outb(PIC1_CTRL_MASK, irq_status);

  kb_buff[buff_items++] = _inb(KB_DATA);

  log_str("INT 0x21: Input scan code = \0");
  log_num(kb_buff[buff_items - 1]);
  log_char('\n');
  if (buff_items >= 32) {
  	log_str("INT 0x21: 32 Characters have been typed.\n\0");
	buff_items = 0;
  }

  irq_status ^= (1 << IRQ_KB);	/* enable PIT IRQ line */
  _outb(PIC1_CTRL_MASK, irq_status);
  _outb(PIC1_CTRL, PIC_ENABLE);		/* enable chips */

  return 1;	/* re-enable interrupts */
}
