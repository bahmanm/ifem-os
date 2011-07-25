/******************************************************************************
 * IFEM OS - kernel/cup.h                                                     * 
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
 * Collection of constants and macros which resemble the cpu internals.
 */
#ifndef KERNEL__CPU_H
#define KERNEL__CPU_H

#define DESC_SIZE	8	/* i386 descriptor size in bytes */
#define GDT_SIZE	16	/* GDT size in rows */
#define IDT_SIZE	64	/* IDT size in rows */

/* Fixed GDT indices. */
#define GDT_RESERVED		0
#define GDT_KERNEL_CS		1
#define GDT_KERNEL_DS		2

/* Fixed GDT selectors. It's index * DESC_SIZE */
#define GDT_KERNEL_CODE		0x0008
#define GDT_KERNEL_DATA		0x0010
#define GDT_INTERFACE_CODE	0x0018
#define GDT_INTERFACE_DATA	0x0020
#define GDT_TASK_DUMMY		0x0028
#define GDT_TASK_KERNEL		0x0030
#define GDT_TASK_PIT		0x0038
#define GDT_TASK_KB		0x0040
#define GDT_INTERFACE_1		0x0048


/* Constants related to i386 segment descriptor attributes and modes. They are 
   stored in two bytes. The format of these bytes is as follows:
   - Lower byte:   P | DPL | DT | Da/Co | E/C | W/R | A
   - Higher bytes: G | D/B | 0 | AVL | limit(16..19)
 */
/* Lower byte: */
#define SDESC_ACCESS	0x01	/* access bit */
#define SDESC_DATA	0x00	/* data segment */
#define SDESC_DA_EDOWN	0x04	/* expand down data segment */
#define SDESC_DA_RW	0x02	/* read/write data segment */
#define SDESC_CODE	0x08	/* code segment */
#define SDESC_CO_CONFORM  0x04	/* conforming code segment */
#define SDESC_CO_RE	0x02	/* read/execute code segment */
#define SDESC_DT	0x00	/* descriptor type: always 0 */
#define SDESC_PRESENT	0x80	/* present bit */

/* Higher byte: */
#define SDESC_AVL	0x10	/* available bit */
#define SDESC_RSVD	0x00	/* reserved 0 bit */
#define SDESC_DEFAULT	0x00	/* code segment operand size */
#define SDESC_GRAN	0x80	/* granullarity bit */


/* Constants related to i386 TSS descriptor attributes and modes. They are 
   stored in two bytes. The format of these bytes is as follows:
   - Lower byte:   P | DPL | 0 | 1 | 0 | B | 1
   - Higher bytes: G | 0 | 0 | AVL | limit(16..19)
 */
/* Lower byte: */
#define TSSDESC_TYPE	0x09	/* reserved type */
#define TSSDESC_BUSY	0x02	/* busy bit */
#define TSSDESC_PRESENT	0x80	/* present bit */

/* Higher byte: */
#define TSSDESC_AVL	0x10	/* available bit */
#define TSSDESC_GRAN	0x80	/* granularity bit */

#define TSKGATE_PRESENT	0x80

/* Descriptor perivilege level. */
#define DESC_KERNEL	0x00
#define DESC_TASK	0x00

/* Privilege levels. */
#define PRIV_KERNEL	0	/* kernel and interrupt routines */
#define PRIV_TASK	0	/* tasks and drivers */


/* Values to mask segment base in a 32 bit variable. */
#define MASK_BASE	0xFFFFFFFFL
#define MASK_BASE_LOW	0x0000FFFFL
#define MASK_BASE_MID	0x00FF0000L
#define MASK_BASE_HIGH	0xFF000000L

/* Values to mask segment base in a 32 bit variable. */
#define MASK_LIMIT	0x000FFFFFL
#define MASK_LIMIT_LOW	0x0000FFFFL
#define MASK_LIMIT_HIGH	0x000F0000L

/* Values to mask segment memory management attributes and privilege level. */
#define MASK_MM		0xF0
#define MASK_PRIV	0x60

/* Constants related to i386 gate descriptors. */
#define IDT_WORD_UNUSED	0x0000
#define IDT_RESERVED	0x0000

/* Type values for IDT gates. The values should be OR'ed with idt.type field. */
#define IDT_TASK	0x05	/* task gate */
#define IDT_INTR	0x0E	/* interrupt gate */
#define IDT_TRAP	0x0F	/* trap gate */

/* These help find a specific place in GDT or IDT. */
#define GDT_FIRST	1	/* GDT first usable index */
#define GDT_NEXT	-1	/* GDT next free & usable index */
#define IDT_FIRST	0	/* IDT first usable index */
#define IDT_NEXT	-1	/* IDT next free & usable index */

#endif /* KERNEL__CPU_H */
