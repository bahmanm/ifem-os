/******************************************************************************
 * IFEM OS - kernel/globvars.h                                                *
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
 * Contains the global variables which are used all over the kernel. This file
 * is automatically included in every kernel source file (see kernel.h).
 */
extern struct segdesc_s *gdt;  /* initialized in 'startup.s' */

/* These are initialized in 'varalloc.c'. */
extern unsigned short  gdt_nr_items;
extern unsigned short  idt_nr_items;

EXTERN struct gatdesc_s idt[IDT_SIZE * DESC_SIZE -1];

EXTERN irq_handler_t irq_table[IRQ_NR_VECT];	/* holds pointers to i8259a IRQ
						   handler routines */
EXTERN unsigned short irq_status;	/* each bit is related to an i8259a IRQ
					   line with the same number;
					   0=enabled, 1=disabled */
EXTERN struct tss_s kernel_tss;
