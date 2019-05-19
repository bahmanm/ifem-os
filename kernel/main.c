/******************************************************************************
 * IFEM OS - kernel/main.c                                                    *
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
 * Contains kernel entry, memory initialization and protected mode setup
 * routines. This file is entered for the first time from 'startup.s'.
 */
#include "kernel.h"

PRIVATE void pit_init(void);
PUBLIC void panic(char *);
PUBLIC void kernel_task_entry(void);
PUBLIC void log_gdt(void);

/*============================================================================*
 *                             kernel_entry                                   *
 *============================================================================*/
void kernel_entry(void)
{
/* This is the entry from which kernel runs for the first time. This happens
   immediately after entering protected mode in 'startup.s' and before doing
   any initialization there.
 */
  char *greet = "Welcome to IFEM 0.1!\n\n\0";
  struct tssdesc_s desc;
  unsigned i;

  log_init();
  log_str(greet);

  log_str("Disabling interrupts...\0");
  _cli();
  log_str("OK\n\0");

  log_str("Initializing IDT and interrupt handlers...\0");
  hwint_init();
  _reload_idt();
  log_str("OK\n\0");

  log_str("Initializing PIC and PIT...\0");
  pic_init();
  pit_init();
  log_str("OK\n\0");

  log_str("Enabling PIT and keyboard IRQ line...\0");
  irq_set_handler(IRQ_PIT, irq_00);
  irq_enable(IRQ_PIT);
  irq_set_handler(IRQ_KB, irq_01);
  irq_enable(IRQ_KB);
  log_str("OK\n\0");

  /* Kernel task */
  log_str("Switching to kernel task...\0");
  setup_tss(&kernel_tss, (task_entry_t)kernel_task_entry,
  	    GDT_KERNEL_CODE, GDT_KERNEL_DATA, 0x09FFF0, 0x0002,
	    0x00, 0x00);
  setup_tss_desc(&desc, &kernel_tss,
  	         TSSDESC_PRESENT | DESC_KERNEL | TSSDESC_TYPE, 0x00);
  i = add2gdt(GDT_DESC &desc, GDT_NEXT);
  _task_run(i);

  /* Unreachable point. */
  panic("kernel_entry(): Unreachable point reached!\n\0");
}

/*============================================================================*
 *				kernel_task_entry			      *
 *============================================================================*/
PUBLIC void kernel_task_entry(void)
{
/* This is the entry point of kernel task. The flow gets here after first task
   switch in kernel_entry().
 */

  log_str("OK\n\0");

  /* STI */
  log_str("Enabling interrupts...\0");
  _sti();
  log_str("OK\n\n\0");

  for (; ;) ;
}

/*============================================================================*
 *				      log_gdt				      *
 *============================================================================*/
PUBLIC void log_gdt(void)
{
  short i;
  register short j;
  short *segp;

  log_clr();
  log_char('\n');
  for (i=0; i<GDT_SIZE; i++) {
  	log_str("GDT Entry #\0"); log_num(i); log_str(":    \0");
	segp = (short *)&gdt_desc(i);
	for (j=0; j<4; j++) {
		log_num(*(segp + j)); log_str(", \0");
	}
	log_char('\n');
  }
}


/*============================================================================*
 *				    pit_init				      *
 *============================================================================*/
PRIVATE void pit_init(void)
{
/* Initialize PIT (programmable interrupt timer). */

  #define PIT_FREQ	1193182L	/* timer clock frequency */
  #define PIT_CNTR_INIT	((unsigned)(PIT_FREQ/FREQ)) /* counter initial value */
  /* i8253 mode word:
     SC1 | SC0 | RL1 | RL0 | M2 | M1 | M0 | BCP

     - SC1, SC0:
         00=counter 0
	 01=counter 1
	 10=counter 2
	 11=illegal value
     - RL1, RL0:
         00=counter value is latched
	 01=read/load least significant byte
	 10=read/load most significant byte
	 11=Read / load least-significant byte first, then most-significant byte
     - M2, M1, M0:
         000=interrupt on terminal count
	 001=programmable one shot
	 x10=rate generator
	 x11=square wave generator
	 100=software triggered strobe
	 101=hardware triggered strobe
     - BCP: 0=binary count, 1=BCD count
   */
  #define PIT_MODE_SQWAVE  0x36

  int i = 0;
  _outb(PIT_CTRL, PIT_MODE_SQWAVE);	/* square wave mode */
  i += 2;
  _outb(PIT_CNTR1, PIT_CNTR_INIT & 0xFF); /* load low byte of counter initial
  					     value */
  i += 2;
  _outb(PIT_CNTR1, PIT_CNTR_INIT >> 8);  /* load high byte of counter initial
  					    value */
}

/*============================================================================*
 *                             	     panic				      *
 *============================================================================*/
PUBLIC void panic(char *str)
{
/* Write the given message to the screen and prompt the user of a panic
   situation.
 */

  _cli();
  log_str("\nPANIC!\n\0");
  log_str(str);
  while (1);
}
