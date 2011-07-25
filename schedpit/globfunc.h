/******************************************************************************
 * IFEM OS - schedpit/globfunc.h                                              * 
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
 * Prototypes all global function used schedpit-wide.
 */
#ifndef SCHEDPIT__GLOBFUNC_H
#define SCHEDPIT__GLOBFUNC_H

/* 
 * asmlib.S
 */
PUBLIC void _sti(void);	 /* enables interrupts */
PUBLIC void _cli(void);  /* diables interrupts */
PUBLIC void _outb(port_t, u8_t);  /* outputs one byte to an I/O port */
PUBLIC u16_t _inb(port_t port); /* inputs a byte from an I/O port */
PUBLIC void _task_run(unsigned);  /* switches to kernel task */
PUBLIC u16_t _ts_flag(void);	/* returns TS (task switched) flag */
PUBLIC void _reset_busy(u16_t);  /* reset busy bit of a TSS descriptor */

/*
 * cpu.c
 */
/* Add descriptors at the requested index to IDT and GDT. */
PUBLIC unsigned add2gdt(struct segdesc_s *, unsigned short);
PUBLIC unsigned add2idt(struct gatdesc_s *, unsigned short);  
/* Setups descriptors. */
PUBLIC void setup_tss_desc(struct tssdesc_s *, struct tss_s *, u8_t, u8_t);
PUBLIC void setup_seg_desc(struct segdesc_s *, u32_t, u32_t, u8_t, u8_t);
/* Setup gates. */
PUBLIC void setup_task_gate(struct gatdesc_s *, u16_t, u8_t);
PUBLIC void setup_call_gate(struct gatdesc_s *, u32_t, u16_t, u8_t, u8_t);
/* Builds up a TSS. */
PUBLIC void setup_tss(struct tss_s *, task_entry_t, u16_t, u16_t, reg_t, reg_t,
		      u16_t, u16_t);
		        
#endif /* SCHEDPIT__GLOBFUNC_H */
