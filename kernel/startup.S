/******************************************************************************
 * IFEM OS - kernel/startup.S                                                 * 
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
 * Does some CPU initilization before kernel itself starts running. It
 * switches to protected mode and loads a minimal GDT and IDT, then calls the
 * kernel entry function.
 */
#include "cpu.h"

# Exported functions.
.global startup
.global _startup

# Exported variables.
.global gdt
.global _gdt
.global p_gdtr

# Imported functions.
.extern kernel_entry

	.code16
	.balign 0x08

startup:
_startup:
	call	write_dot	# update progress bar

	movw	%cs, %ax	# setup DS and ES
	movw	%ax, %ds
	movw	%ax, %es

	# Load our own GDT.
	cli			# no interrupts allowed
	lgdt	p_gdtr
	call	write_dot	# update progress bar

	# Switch to protected mode by setting the LSB of CR0.
	movl	%cr0, %eax
	orl	$0x00000001, %eax
	movl	%eax, %cr0
	
	# Clear NT (Nested Task) mode flag.
	pushf
	popw	%ax
	andw	$0xBFFF, %ax
	pushw	%ax
	popf

	# Welcome to protected mode! First empty the CPU intsruction cache by
	# performing a jump and then perform a long jump to the protected code.
	#
	jmp	empty_cache
empty_cache:
	.byte	0x66, 0xea
	.long	pm_begin
	.word	GDT_KERNEL_CODE

	# 16 bit procedures.
	#######################################################################
	#				write_dot			      #
	#######################################################################
	# Write a '.' to the screen.
	#
	.balign	0x08
write_dot:
	pushw	%ax
	movb	$('.'), %al
	movb	$0x0e, %ah
	int	$0x10
	popw	%ax

	ret

	# Protected code section.
	#
	.code32
pm_begin:
	# Setup segments.
	movw	$GDT_KERNEL_DATA, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	movl	$0x09fff0, %eax
	movl	%eax, %esp
	
	# Load a dummy task.This is needed to switch to kernel task later.
	movw	$GDT_TASK_DUMMY, %ax
	ltr	%ax
	
	# Jump to kernel entry & initializtion routine.
	call	kernel_entry
		
eternity:
	# If everything goes right, this is unreachable.
	jmp	eternity


	#######################################################################
	#			    Data Section			      #
	#######################################################################
	# Define GDT.
	.balign 0x08
		.word	0x0000
p_gdtr:		.word	(GDT_SIZE * 8) - 1
		.long	p_gdt
		
	.balign 0x08
p_gdt:
		.word	0x0000, 0x0000, 0x0000, 0x0000  # system reserved
		.word	0xFFFF, 0x0000, 0x9A00, 0x00CF  # kernel code
		.word	0xFFFF, 0x0000, 0x9200, 0x00CF  # kernel data
		.word	0xFFFF, 0x0000, 0x9A00, 0x00CF  # interfaces code
		.word	0xFFFF, 0x0000, 0x9200, 0x00CF  # interfaces data
		.word	0x0068, 0x0000, 0x8900, 0x0000  # dummy task; used to
							# switch to kernel task
		.rept	GDT_SIZE - 6
		.word	0x0000, 0x0000, 0x0000, 0x0000
		.endr
		
	# Declare variables.
	.data
gdt:
_gdt:		.long	p_gdt
	.end
