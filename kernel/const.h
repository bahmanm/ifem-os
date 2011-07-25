/******************************************************************************
 * IFEM OS - kernel/const.h                                                   * 
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
 * Collection of macros and constants which are not explicitly related to a
 * specific part of the kernel.
 */
#ifndef KERNEL__CONST_H
#define KERNEL__CONST_H

/* These macros help working with 'gdt' and 'idt' global variables. */
#define gdt_desc(i)	gdt[(i)]	/* GDT item #i */
#define idt_gate(i)	idt[(i)]	/* IDT item #i */
#define GDT_DESC	(struct segdesc_s *)  /* to cast the other descriptors
					       when using add2gdt() */

/* Macros for word and byte size. */
#define _B	_BYTE_SIZE	/* byte size in bits */
#define _W	_WORD_SIZE	/* word size in bytes */

/* i8253 PIT (programmable interrupt and timer) ports and commands. */
#define PIT_CNTR1	0x40	/* counter 1 */
#define PIT_CNTR2	0x41	/* counter 2 */
#define PIT_CNTR3	0x42	/* counter 3 */
#define PIT_CTRL	0x43	/* control register */

/* i8259a programmable interrupt controller ports & commands. */
#define PIC1_CTRL	0x20	/* i8259a I/O port */
#define PIC1_CTRL_MASK	0x21	/* to disable an interrupt, just set its related
				   bit in this port */
#define PIC2_CTRL	0xA0	/* same as PIC1_CTRL but for second interrupt 
				   controller */
#define PIC2_CTRL_MASK	0xA1	/* same as PIC1_CTRL_MASK but for second
				   interrupt controller */

#define PIC_ENABLE	0x20	/* this is used to re-enable interrupts after an
				   interrupt */
				   

#define IRQ0_VECT	0x20	/* vector# for IRQ0 handler (master chip) */	
#define IRQ8_VECT	0x28	/* vector# for IRQ8 handler (slave chip) */

#define IRQ_NR_VECT	16	/* # of IRQ vectors */
#define IRQ_PIT		0	/* programmable interrupt timer (clock) */
#define IRQ_KB		1	/* keyboard */
#define IRQ_CASCADE	2	/* master IRQ# on which the slave interrupts
				   should be cascaded */
#define IRQ_ETHERNET	3	/* ethernet */
#define IRQ_RS232_2	3	/* RS232 port 2 */
#define IRQ_RS232_1	4	/* RS232 port 1 */
#define IRQ_FLOPPY	6	/* floppy disk drive */
#define IRQ_PRINTER	7	/* printer */
#define IRQ_WINCH_AT	14	/* AT winchester (hd controller) */

#define __FOREVER__	{ for (; ;); }

#endif
