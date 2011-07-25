/******************************************************************************
 * IFEM OS - kernel/asmlib.S                                                  * 
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
 * Kernel assembly library.
 */
#include "cpu.h"

# Exported functions.
#
.global		_sti
.global		__sti

.global		_cli
.global		__cli

.global		_reload_gdt
.global		__reload_gdt

.global		_reload_idt
.global		__reload_idt

.global		_save_idt
.global		__save_idt

.global		_outb
.global		__outb

.global		_inb
.global		__inb

.global		_task_run
.global		__task_run

.global		_ts_flag
.global		__ts_flag

.global		_reset_busy
.global		__reset_busy

	.text
	.code32
	
	########################################################################
	#                               _sti				       #
	########################################################################
	# PUBLIC void _sti(void);
	# Enable intrrupts.
	#
	.balign 0x08
_sti:
__sti:
	sti
	ret
	
	########################################################################
	#                               _cli				       #
	########################################################################
	# PUBLIC void _cli(void);
	# Disable interrupts.
	#
	.balign 0x08
_cli:
__cli:
	cli
	ret

	########################################################################
	#			     _reload_gdt                               #
	########################################################################
	# PUBLIC void _reload_gdt(void);
	# Reloads 'pgdt_r' global variable to CPU GDTR.
	# All interrupts should be disabled before using this function.
	#
	.balign 0x08
_reload_gdt:
__reload_gdt:
	lgdt	p_gdtr
	
	ret
	
	########################################################################
	#			     _reload_idt                               #
	########################################################################
	# PUBLIC void _reload_idt(void);
	# Reloads 'idt' global variable to CPU IDTR.
	# Disable all interrupts before you use this functions.
	#
	.balign 0x08
_reload_idt:
__reload_idt:
	lidt	_idtr
	
	ret
	
	.balign	0x08
	.word	0x0000
_idtr:	.word	(IDT_SIZE * DESC_SIZE) - 1
	.long	idt
	
	########################################################################
	#			       _save_idt                               #
	########################################################################
	# PUBLIC void _save_idt(void);
	# saves CPU IDTR to 'idt' global variable.
	#
	.balign 0x08
_save_idt:
__save_idt:
	sidt	_idtr
	
	ret
	
	########################################################################
	#			         _outb				       #
	########################################################################
	# PUBLIC void _outb(port_t port, u8_t val)
	# Outputs 'value' to I/O port 'port'.
	#
	.balign	0x08
_outb:
__outb:
	pushw	%dx
	pushw	%ax
	
	movw	8(%esp), %dx	# port
	movb	12(%esp), %al	# val
	outb	%dx		# output val to port
	
	popw	%ax
	popw	%dx
	
	ret
		
	########################################################################
	#				  _inb				       #
	########################################################################
	# PUBLIC u16_t _inb(port_t port);
	# Inputs a byte from I/O port 'port'.
	#
	.balign	0x08
_inb:
__inb:
	pushw	%dx
	
	movw	6(%esp), %dx	# port
	subl	%eax, %eax
	inb	%dx		# input from port
	
	popw	%dx
	
	ret
	
	########################################################################
	#			       _task_run			       #
	########################################################################
	# PUBLIC void _kerneltask_run(unsigned sltr);
	# Switch to kernel task whose TSS selector is in GDT index 'sltr'. This
	# is done only once and, in fact, should be implemented as a macro but
	# this way it will not fall into compiler compatability fuss when
	# compiling using something other than GCC.
	#
_task_run:
__task_run:
	pushl	%esi
	movl	8(%esp), %eax	# getting 'sltr'
	movl	tmp, %esi
	movl	%eax, 4(%esi)
	movl	%esi, %eax
	ljmp	*(%eax)		# switch, now!
	popl	%esi
	
	ret
	
	.balign	0x08
tmp:		.long	0, 0

	########################################################################
	#				 _ts_flag			       #
	########################################################################
	# PUBLIC u16_t _ts_flag(void);
	# Return 1 if TS=1 and 0 if TS=0.
	# TS (task switched) flag is located in CR0 bit 3.
	#
_ts_flag:
__ts_flag:
	cli
	movl	%cr0, %eax
	andl	$0x08, %eax
	shrl	$3, %eax
	pushl	%eax		# push TS 
	movl	%cr0, %eax	# 0 TS
	andl	$0xFFFFFFF7, %eax
	movl	%eax, %cr0
	popl	%eax		# return value
	sti
	
	ret

	########################################################################
	#			      _reset_busy			       #
	########################################################################
	# PUBLIC void _reset_busy(u16_t sltr);
	# Reset 'sltr' busy bit.
	# - sltr: TSS descriptor located in GDT.
	#
_reset_busy:
__reset_busy:
	pushfl
	subl	%eax, %eax
	movw	8(%esp), %ax	# %ax = sltr
	shrw	$3, %ax		# %ax = index of descriptor
	pushl	%esi
	movl	gdt, %esi
	shlw	$3, %ax		# each descriptor is 4 bytes long
	addl	%eax, %esi	# %esi = address of the descriptor
	andw	$0xFDFF, 4(%esi)  # busy bit is bit 9
	popfl
	popl	%esi
	
	ret
	
	.end
