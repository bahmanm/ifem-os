/******************************************************************************
 * IFEM OS - kernel/intrface.c                                                * 
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
 * Kernel interfaces for higher levels are implemented via i386 'call gates'.
 * This file contains the interfaces.
 * Argument list of all function begin and end with a u32_t. The beginning
 * u32_t is a place holder for i386. When a call gate is invoked i386 pushes
 * return address into stack, so we should take care of it. (It's strange, but
 * this point is not documented by Intel.) The ending u32_t is a pointer to a 
 * variable in the caller task which will contain the return value of the
 * operation, whether it is successful or not. This is due to the fact that 
 * call gates does not support return values.
 * 
 * When trying to invoke a call gate which has pointers as its arguments,
 * remember to use a (void *) variable to pass the pointer. For example:
 *    sometype_t *s;
 *    void *p;
 *    p = s;
 *    call_gate(.., p, ...)
 * 's' is the variable you want to pass its address but you have to do so
 *  using 'p'. I don't believe it's the only way but I don't know any other.
 *  
 * The functions below, in fact, do nothing except calling other functions
 * and most of those other functions do not check the validity of their 
 * inputs. They assume that the arguments are always valid. Maybe we should 
 * implement some validity check here?
 */
#include "kernel.h"

/* Add descriptors at the requested index to IDT and GDT. */
PUBLIC void i_add2gdt(u32_t,
		      struct segdesc_s *, unsigned short,
		      u32_t);
PUBLIC void i_add2idt(u32_t,
		      struct gatdesc_s *, unsigned short,
		      u32_t);
			
/*============================================================================*
 *				    i_add2gdt				      *
 *============================================================================*/
PUBLIC void i_add2gdt(u32_t dummy,
		      struct segdesc_s *desc, unsigned short index,
		      u32_t errcodep)
{
/* Invokes add2gdt() in 'cpu.c'.
   - dummy: Placeholder for return address.
   - desc: Descriptor to add to GDT.
   - index: Place at where it should be placed.
   - errcodep: Pointer to storage for return value.
 */
 
  *(unsigned *)errcodep = add2gdt(desc, index);
}

/*============================================================================*
 *				     i_add2idt				      *
 *============================================================================*/
PUBLIC void i_add2idt(u32_t dummy,
		      struct gatdesc_s *desc, unsigned short index,
		      u32_t errcodep)
{
/* Invokes add2idt() in 'cpu.c'.
   - dummy: Placeholder for return address.
   - desc: Descriptor to add to IDT.
   - index: Place at where it should be placed.
   - errcodep: Pointer to storage for return value.
 */
 
  *(unsigned *)errcodep = add2idt(desc, index);
}
