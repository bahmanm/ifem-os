/******************************************************************************
 * IFEM OS - kernel/cpu.c                                                     * 
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
 * This file defines function which are heavily CPU dependent.
 */
#include "kernel.h"

/* Setup descriptors. */
PUBLIC void setup_tss_desc(struct tssdesc_s *, struct tss_s *, u8_t, u8_t);
/* Setup gates. */
PUBLIC void setup_task_gate(struct gatdesc_s *, u16_t, u8_t);
PUBLIC void setup_call_gate(struct gatdesc_s *, u32_t, u16_t, u8_t, u8_t);
PUBLIC void setup_int_gate(struct gatdesc_s *, u32_t, u16_t, u8_t);
/* Add descriptors at the requested index to IDT and GDT. */
PUBLIC unsigned add2gdt(struct segdesc_s *, unsigned short);
PUBLIC unsigned add2idt(struct gatdesc_s *, unsigned short);
/* Setups a TSS. */
PUBLIC void setup_tss(struct tss_s *, task_entry_t, u16_t, u16_t, reg_t, reg_t,
		      u16_t, u16_t);


/*============================================================================*
 *				    add2gdt				      *
 *============================================================================*/
PUBLIC unsigned add2gdt(struct segdesc_s *desc, unsigned short index)
{
/* Adds a new descriptor to 'gdt' global variable at 'index' place. Then reloads
   GDTR.
   - desc: Descriptor to add to GDT.
   - index: Place at where it should be placed.
 */
  register unsigned short j;
  unsigned short i;
  register char *segp;
  
  if ((short)index == GDT_NEXT) 
  	i = gdt_nr_items++;	/* next free usable index */
  else 
  	i = index;
	
  /* Copy 'desc' to ith index of GDT. */
  segp = (char *)&gdt_desc(i);
  for (j=0; j<sizeof(struct segdesc_s); j++)
  	*(segp + j) = *((char *)desc + j);

  return (i << 3);	/* selector of the newly added descriptor */
}

/*============================================================================*
 *				    add2idt				      *
 *============================================================================*/
PUBLIC unsigned add2idt(struct gatdesc_s *gate, unsigned short index)
{
/* Adds a new gate to 'idt' global variable at 'index' place. Then reloads
   IDTR.
   - gate: Gate to add to IDT.
   - index: Place at where it should be placed.
 */
  register unsigned short j;
  unsigned short i;
  register char *gatp;
  
  if ((short)index == IDT_NEXT) 
  	i = idt_nr_items++;	/* next free usable index */
  else 
  	i = index;
	
  /* Copy 'gate' to ith index of IDT. */
  gatp = (char *)&idt_gate(i);
  for (j=0; j<sizeof(struct gatdesc_s); j++)
  	*(gatp + j) = *((char *)gate + j);
	
  return i;	/*index of the newly added gate */
}

/*============================================================================*
 *				      setup_tss				      *
 *============================================================================*/
PUBLIC void setup_tss(struct tss_s *tss, task_entry_t entry, u16_t cs, u16_t ds,
		      reg_t esp, reg_t eflags, u16_t trap, u16_t iomap)
{
/* Builds up a TSS.
   - tss: Place where the TSS is built.
   - entry: Offset in code segment from which the task code runs for 
     the first time that the task is entered.
   - cs, ds, esp and eflags: Values for their respective registers.
   - trap: Determines if debug trap bit is set or not.
   - iomap: Base address of I/O map.
 */

  tss->backlink = 0x00;
  tss->esp0 = esp;
  tss->ss0 = ds;
  tss->esp1 = 0x00;
  tss->ss1 = 0x00;
  tss->esp2 = 0x00;
  tss->ss2 = 0x00;
  tss->cr3 = 0x00;
  tss->eip = (reg_t)entry;
  tss->eflags = eflags;
  tss->eax = 0x00;
  tss->ecx = 0x00;
  tss->edx = 0x00;
  tss->ebx = 0x00;
  tss->esp = esp;
  tss->ebp = 0x00;
  tss->esi = 0x00;
  tss->edi = 0x00;
  tss->es = ds;
  tss->cs = cs;
  tss->ss = ds;
  tss->ds = ds;
  tss->fs = ds;
  tss->gs = ds;
  tss->ldt = 0x00;
  tss->trap = 0x00;
  tss->iomap_base = 0x00;
}

/*============================================================================*
 *				  setup_tss_desc			      *
 *============================================================================*/
PUBLIC void setup_tss_desc(struct tssdesc_s *desc, struct tss_s *tss, u8_t type,
			   u8_t mm_limit)
{
/* Setups a TSS descriptor to be placed in GDT.
   - desc: Descriptor to be filled.
   - tss: Address of TSS for which this descriptor is built.
   - type and mm_limit: Values for their respective fields. 
 */

  register reg_t tss_size = sizeof(struct tss_s);
  desc->limit_low = (u16_t)tss_size;
  desc->mm_limit = (mm_limit & 0xF0) | ((tss_size >> 16) & 0x0F);
  desc->base_low = (u16_t)((u32_t)tss & 0xFFFF);
  desc->base_mid = (u8_t)(((u32_t)tss & 0x00FF0000) >> 16);
  desc->base_high = (u8_t)(((u32_t)tss & 0xFF000000) >> 24);
  desc->type = type;
}

/*============================================================================*
 *				  setup_task_gate			      *
 *============================================================================*/
PUBLIC void setup_task_gate(struct gatdesc_s *gate, u16_t sltr, u8_t type)
{
/* Setups a task gate to be placed in IDT.
   - gate: Descriptor to be filled.
   - sltr: Selector of TSS descriptor. 
   - type: Value for its respective field. 
 */
 
  gate->selector = sltr;
  gate->type = type;
}

/*============================================================================*
 *				   setup_call_gate			      *
 *============================================================================*/
PUBLIC void setup_call_gate(struct gatdesc_s *gate, u32_t entry, u16_t sltr,
			    u8_t param_dwords, u8_t type)
{
/* Setups a call gate to be placed in GDT.
   - gate: Descriptor to be filled.
   - entry: Offset of function to be called.
   - sltr: Selector of code segment.
   - param_dwords: Count of double words to copy from caller stack to dest
     stack.
   - type: Value for its respective field.
 */
   
  gate->offset_low = (u16_t)entry;
  gate->offset_high = (u16_t)(entry >> 16);
  gate->selector = sltr;
  gate->type = type;
  gate->dword_count = param_dwords & 0x1F;
}

/*============================================================================*
 *				   setup_int_gate			      *
 *============================================================================*/
PUBLIC void setup_int_gate(struct gatdesc_s *gate, u32_t entry, u16_t sltr,
			   u8_t type)
{
/* Setups an interrupt gate to be places in IDT.
   - gate: Descriptor to be filled.
   - entry: Offset of function to be called.
   - sltr: Selector of code segment.
   - type: Value for its respective field.
 */

  gate->offset_low = (u16_t)entry;
  gate->offset_high = (u16_t)(entry >> 16);
  gate->selector = sltr;
  gate->type = type;
  gate->dword_count = 0x00;
}
