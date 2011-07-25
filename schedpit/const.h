/******************************************************************************
 * IFEM OS - schedpit/const.h                                                 * 
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
 * Contains constants and macros used in schedpit module.
 */
#ifndef SCHEDPIT__CONST_H
#define SCHEDPIT__CONST_H

#define SCHEDPIT_STACK_SIZE  1024  /* stack size for 'schedpit' */

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
				   
/* i8253 PIT (programmable interrupt and timer) ports and commands. */
#define PIT_CNTR1	0x40	/* counter 1 */
#define PIT_CNTR2	0x41	/* counter 2 */
#define PIT_CNTR3	0x42	/* counter 3 */
#define PIT_CTRL	0x43	/* control register */

#define IRQ0_VECT	0x20	/* vector# for IRQ0 handler (master chip) */	
#define IRQ8_VECT	0x28	/* vector# for IRQ8 handler (slave chip) */
#define IRQ_PIT		0	/* IRQ line for i8253 PIT */

#define __FOREVER__	{ for (; ;); }

#endif /* SCHEDPIT__CONST_H */
