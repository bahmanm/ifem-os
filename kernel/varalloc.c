/******************************************************************************
 * IFEM OS - kernel/varalloc.c                                                * 
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
 * Allocates memory for all EXTERN-defined variables.
 */
#define _VARALLOC

#include "kernel.h"

unsigned short gdt_nr_items = 6; /* Holds the number of items in GDT. The 
				    initial value should be synchronized with
				    the number of items which 'startup.s' put
				    into GDT. */
unsigned short idt_nr_items = 0; /* holds the number of items in IDT */

