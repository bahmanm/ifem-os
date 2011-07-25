/******************************************************************************
 * IFEM OS - kernel/segconst.S                                                * 
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
 * Assembly level constants.
 */
	# Bootloader constants.
	.set	K_SEG, 0x0	  # kernel segment
	.set	K_OFF, 0x0000600  # kernel offset
	
	# Protected mode constants.
	# These constants are also defined in 'cpu.h'. So if you edit either,
	# you should update the other file.
	#
	.set	GDT_SIZE, 16	  	   # GDT size in rows
	.set	IDT_SIZE, 32		   # IDT size in rows
	.set	GDT_SEL_KERNEL_CS, 0x0008  # kernel code selector
	.set	GDT_SEL_KERNEL_DS, 0x0010  # kernel data selector
