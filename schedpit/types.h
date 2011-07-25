/******************************************************************************
 * IFEM OS - schedpit/types.h                                                 * 
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
 * Defines types that are merely used in schedpit.
 */
#ifndef SCHEDPIT__TYPES_H
#define SCHEDPIT__TYPES_H

typedef unsigned reg_t;

/* Note that 'segdesc_s' and 'tssdesc_s' are identical but they have been
   defined in two separate structures, for ease of use.
 */
struct segdesc_s {
/* Defines i386 segment descriptor, used in GDT and LDT. */
	u16_t limit_low;	/* segment limit, bits(0..15) */
	u16_t base_low;		/* segment base, bits(0..15) */	
	u8_t base_mid;		/* segment base, bits(16..23) */
	u8_t type;		/* Segment type and attributes:
				   P | DPL | DT | Da/Co | E/C | W/R | A */
	u8_t mm_limit;		/* Segment memory management attributes and high
				   limit bits:
				   G | D/B | 0 | AVL | limit(16..19) */
	u8_t base_high;		/* segment base, bits(24..31) */
};

struct tssdesc_s {
/* Defines i386 TSS (task state segment) descriptor used in GDT. */
	u16_t limit_low;	/* segment limit, bits(0..15) */
	u16_t base_low;		/* segment base, bits(0..15) */	
	u8_t base_mid;		/* segment base, bits(16..23) */
	u8_t type;		/* Segment type and attributes:
				   P | DPL | 0 | 1 | 0 | B | 1 */
	u8_t mm_limit;		/* Segment memory management attributes and high
				   limit bits:
				   G | 0 | 0 | AVL | limit(16..19) */
	u8_t base_high;		/* segment base, bits(24..31) */
};

struct gatdesc_s {
/* Desfines i386 gate descriptor, used in IDT & GDT. */
	u16_t offset_low;	/* Used in interrupt, trap and call gates; not
				   used in task gates. Determines the lower word
				   of offset of the code to be executed. */
	u16_t selector;		/* segment selector for the code */
	u8_t dword_count;	/* In interrupt and trap gates bits(7..5)=0 and
				   the rest is not used; In task gates the whole
				   byte is not used. In call gates bits(7..5)=0
				   and bits(4..0) determines the double word 
				   count to copy to stack. */
	u8_t type;		/* P | DPL | TYPE */	
	u16_t offset_high;	/* Used in interrupt, trap and call gates; not
				   used in task gates. Determines the higher 
				   word of offset of the code to be executed. */
};

typedef u32_t task_entry_t;	/* address of task entry function */

struct tss_s {
/* Defines i386 TSS (task state segment).
   Rest of the fields which part of them is used should be filled with 0. */
	reg_t	backlink;	/* back link to previous TSS */
	reg_t	esp0;
	reg_t	ss0;		/* only lower 16 bits used */
	reg_t	esp1;
	reg_t	ss1;		/* only lower 16 bits used */
	reg_t	esp2;
	reg_t	ss2;		/* only lower 16 bits used */
	reg_t	cr3;		/* PDBR (page directory base register) */
	reg_t	eip;
	reg_t	eflags;
	reg_t	eax;
	reg_t	ecx;
	reg_t	edx;
	reg_t	ebx;
	reg_t	esp;
	reg_t	ebp;
	reg_t	esi;
	reg_t	edi;
	reg_t	es;		/* only lower 16 bits used */
	reg_t	cs;		/* only lower 16 bits used */
	reg_t	ss;		/* only lower 16 bits used */
	reg_t	ds;		/* only lower 16 bits used */
	reg_t	fs;		/* only lower 16 bits used */
	reg_t	gs;		/* only lower 16 bits used */
	reg_t	ldt;		/* only lower 16 bits used */
	u16_t	trap;		/* bit 0 used only: debug trap bit */
	u16_t	iomap_base;
};


#endif /* SCHEDPIT__TYPES_H */
