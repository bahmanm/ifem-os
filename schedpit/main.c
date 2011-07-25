/******************************************************************************
 * IFEM OS - schedpit/main.c                                                  * 
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
 * This file contains the 'schedpit' module initialization routine.
 */
#include "schedpit.h"

PUBLIC void schedpit_init(unsigned); /* intializes 'schedpit' task */
PUBLIC void schedpit_task_entry(void);  /* schedpit entry */

/*============================================================================*
 *				  schedpit_init				      *
 *============================================================================*/
PUBLIC void schedpit_init(unsigned code_base)
{
/* Setups 'schedpit' task.
   Creates a TSS, add a TSS descriptor to GDT and add an task gate to IDT.
   - code_base: Starting address of 'schedpit' code and data read from address
     0x05FC by kernel. 'schedpit' segment descriptors should have this value as
     their 'base' field.
 */
  
  /*register unsigned short *vga = 0xB8000L;
  
  *vga = 0xF440;*/
  struct segdesc_s desc;
  struct gatdesc_s gate;
  unsigned cs, ds, tss;
  reg_t esp;
  
  /* Setup code segment descriptor. */
  setup_seg_desc(&desc, code_base, 0x0FFFFF,
  		 SDESC_PRESENT | SDESC_DT | SDESC_CODE | SDESC_CO_RE, /* type */
		 SDESC_AVL | SDESC_GRAN | SDESC_BIGBIT /* mm */);
  cs = add2gdt(&desc, GDT_NEXT);
  
  /* Setup data segment descriptor. */
  setup_seg_desc(&desc, code_base, 0xFFFFF,
  		 SDESC_PRESENT | SDESC_DT | SDESC_DATA | SDESC_DA_RW, /* type */
		 SDESC_AVL | SDESC_GRAN | SDESC_BIGBIT /* mm */);
  ds = add2gdt(&desc, GDT_NEXT);
  
  /* Setup TSS. */
  esp = (reg_t)schedpit_stack + SCHEDPIT_STACK_SIZE;
  setup_tss(&schedpit_tss, (task_entry_t)schedpit_task_entry,
  	    cs, ds, esp, 0x0002, 0x00, 0x00);
  
  /* Setup TSS descriptor. */
  setup_tss_desc((struct tssdesc_s *)&desc, &schedpit_tss,
  		 TSSDESC_PRESENT | DESC_TASK | TSSDESC_TYPE, 0x00);
  
  tss = add2gdt(&desc, GDT_NEXT);
  
  /* Setup task gate descriptor. */
  setup_task_gate(&gate, tss, TSKGATE_PRESENT | DESC_TASK | IDT_TASK);
  add2idt(&gate, IRQ0_VECT + IRQ_PIT);
  
  /**** PROCS ARRAY INIT ****/
}
 
/*============================================================================*
 *				pit_task_entry				      *
 *============================================================================*/
PUBLIC void schedpit_task_entry(void)
{
  for (; ;)
  	if (_ts_flag() == 1) {
	  /*irq_00(0x20);*/
	} else {

	  /*_reset_busy(schedpit_tss.backlink);
	    _task_run(schedpit_tss.backlink);*/
	}
}
