/******************************************************************************
 * IFEM OS - kernel/isr.S                                                     * 
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
 * Here all the primary interrupt handlers are defined. In fact, most of the
 * time, these routines do nothing, except modifying a few variables; but in
 * certain conditions they call another routine which is located outside of
 * the kernel. It tells the scheduler, which itself is also outside of the 
 * kernel, to schedule the main interrupt handler routine, if any exists.
 */
# Imported variables.
#
.extern	irq_table
.extern	gdt
.extern idt

# Imported functions.
#
.extern	int_dispatch
		
# Exported functions.
#
.global	_hwint_00
.global __hwint_00

.global	_hwint_01
.global __hwint_01

.global	_hwint_02
.global __hwint_02

.global	_hwint_03
.global __hwint_03

.global	_hwint_04
.global __hwint_04

.global	_hwint_05
.global __hwint_05

.global	_hwint_06
.global __hwint_06

.global	_hwint_07
.global __hwint_07

.global	_hwint_08
.global __hwint_08

.global	_hwint_09
.global __hwint_09

.global	_hwint_0A
.global __hwint_0A

.global	_hwint_0B
.global __hwint_0B

.global	_hwint_0C
.global __hwint_0C

.global	_hwint_0D
.global __hwint_0D

.global	_hwint_0E
.global __hwint_0E

.global	_hwint_0F
.global __hwint_0F

.global _hwint_10
.global __hwint_10

.global _hwint_11
.global __hwint_11

.global _hwint_12
.global __hwint_12

.global _hwint_13
.global __hwint_13

.global _hwint_14
.global __hwint_14

.global _hwint_15
.global __hwint_15

.global _hwint_16
.global __hwint_16

.global _hwint_17
.global __hwint_17

.global _hwint_18
.global __hwint_18

.global _hwint_19
.global __hwint_19

.global _hwint_1A
.global __hwint_1A

.global _hwint_1B
.global __hwint_1B

.global _hwint_1C
.global __hwint_1C

.global _hwint_1D
.global __hwint_1D

.global _hwint_1E
.global __hwint_1E

.global _hwint_1F
.global __hwint_1F

.global _hwint_20
.global __hwint_20

.global _hwint_21
.global __hwint_21

.global _hwint_22
.global __hwint_22

.global _hwint_23
.global __hwint_23

.global _hwint_24
.global __hwint_24

.global _hwint_25
.global __hwint_25

.global _hwint_26
.global __hwint_26

.global _hwint_27
.global __hwint_27

.global _hwint_28
.global __hwint_28

.global _hwint_29
.global __hwint_29

.global _hwint_2A
.global __hwint_2A

.global _hwint_2B
.global __hwint_2B

.global _hwint_2C
.global __hwint_2C

.global _hwint_2D
.global __hwint_2D

.global _hwint_2E
.global __hwint_2E

.global _hwint_2F
.global __hwint_2F

.global _hwint_30
.global __hwint_30

.global _hwint_31
.global __hwint_31

.global _hwint_32
.global __hwint_32

.global _hwint_33
.global __hwint_33

.global _hwint_34
.global __hwint_34

.global _hwint_35
.global __hwint_35

.global _hwint_36
.global __hwint_36

.global _hwint_37
.global __hwint_37

.global _hwint_38
.global __hwint_38

.global _hwint_39
.global __hwint_39

.global _hwint_3A
.global __hwint_3A

.global _hwint_3B
.global __hwint_3B

.global _hwint_3C
.global __hwint_3C

.global _hwint_3D
.global __hwint_3D

.global _hwint_3E
.global __hwint_3E

.global _hwint_3F
.global __hwint_3F

	.text
	.code32
	
################################################################################	
# This macro expands to a primary interrupt handler routine for master chip IRQ.
#define hw_master_isr(irq)						      #\
	.balign	0x08							      #\
1:	cli		   						      #\
	pushal								      #\
	pushw	$irq							      #\
	call	(irq_table + 4*(irq))	/* irq_table[irq](irq) */	      #\
	sti								      #\
	popal								      #\
	iret								      #\
	jmp	0b
	
################################################################################
# This macro expands to a primary interrupt handler routine for slave chip IRQ.

#define hw_slave_isr(irq)						      #\
	.balign	0x08							      #\
0:	cli								      #\
	pushw	%ax							      #\
	push	irq							      #\
	call	(irq_table + 4*(irq))	/* irq_table[irq](irq) */	      #\
	sti								      #\
	popw	%ax							      #\
	iret								      #\
	jmp	0b

################################################################################
# This macro expands to a primary iterrupt handler for execptions and any other
# kind of interrupt, except 8259's ones.

.macro int_handler int_no
1:	cli 
	pushal
	pushl $&int_no
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
.endm							      
	
################################################################################
# Primary interrupt service routines for IRQ0 to IRQ63

_hwint_00:
__hwint_00:
1:	cli 
	pushal
	pushl $0x00
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_01:
__hwint_01:
1:	cli 
	pushal
	pushl $0x01
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
		
_hwint_02:
__hwint_02:
1:	cli 
	pushal
	pushl $0x02
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_03:
__hwint_03:
1:	cli 
	pushal
	pushl $0x03
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_04:
__hwint_04:
1:	cli 
	pushal
	pushl $0x04
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_05:
__hwint_05:
1:	cli 
	pushal
	pushl $0x05
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_06:
__hwint_06:
1:	cli 
	pushal
	pushl $0x06
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_07:
__hwint_07:
1:	cli 
	pushal
	pushl $0x07
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_08:
__hwint_08:
1:	cli 
	pushal
	pushl $0x08
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_09:
__hwint_09:
1:	cli 
	pushal
	pushl $0x09
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_0A:
__hwint_0A:
1:	cli 
	pushal
	pushl $0x0A
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_0B:
__hwint_0B:
1:	cli 
	pushal
	pushl $0x0B
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_0C:
__hwint_0C:
1:	cli 
	pushal
	pushl $0x0C
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_0D:
__hwint_0D:
1:	cli 
	pushal
	pushl $0x0D
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_0E:
__hwint_0E:
1:	cli 
	pushal
	pushl $0x0E
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_0F:
__hwint_0F:
1:	cli 
	pushal
	pushl $0x0F
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_10:
__hwint_10:
1:	cli 
	pushal
	pushl $0x10
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_11:
__hwint_11:
1:	cli 
	pushal
	pushl $0x11
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	        
_hwint_12:
__hwint_12:
1:	cli 
	pushal
	pushl $0x12
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_13:
__hwint_13:
1:	cli 
	pushal
	pushl $0x13
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_14:
__hwint_14:
1:	cli 
	pushal
	pushl $0x14
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_15:
__hwint_15:
1:	cli 
	pushal
	pushl $0x15
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_16:
__hwint_16:
1:	cli 
	pushal
	pushl $0x16
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_17:
__hwint_17:
1:	cli 
	pushal
	pushl $0x17
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_18:
__hwint_18:
1:	cli 
	pushal
	pushl $0x18
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_19:
__hwint_19:
1:	cli 
	pushal
	pushl $0x19
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_1A:
__hwint_1A:
1:	cli 
	pushal
	pushl $0x1A
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_1B:
__hwint_1B:
1:	cli 
	pushal
	pushl $0x1B
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_1C:
__hwint_1C:
1:	cli 
	pushal
	pushl $0x1C
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_1D:
__hwint_1D:
1:	cli 
	pushal
	pushl $0x1D
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_1E:
__hwint_1E:
1:	cli 
	pushal
	pushl $0x1E
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_1F:
__hwint_1F:
1:	cli 
	pushal
	pushl	$0x1F
	call 	int_dispatch
	addl  	$0x04, %esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_20:
__hwint_20:
1:	cli 
	pushal
	pushl 	$0x20
	call 	int_dispatch
	addl  	$0x04, %esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_21:
__hwint_21:
1:	cli 
	pushal
	pushl	$0x21
	call 	int_dispatch
	addl	$0x04,	%esp
	#inb	$0x60
	#movb	$0x20, %al
	#outb	$0x20
	popal
	sti
	iret
	jmp 1b
	
_hwint_22:
__hwint_22:
1:	cli 
	pushal
	pushl $0x22
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_23:
__hwint_23:
1:	cli 
	pushal
	pushl $0x23
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_24:
__hwint_24:
1:	cli 
	pushal
	pushl $0x24
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_25:
__hwint_25:
1:	cli 
	pushal
	pushl $0x25
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_26:
__hwint_26:
1:	cli 
	pushal
	pushl $0x26
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_27:
__hwint_27:
1:	cli 
	pushal
	pushl $0x27
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_28:
__hwint_28:
1:	cli 
	pushal
	pushl $0x28
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_29:
__hwint_29:
1:	cli 
	pushal
	pushl $0x29
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_2A:
__hwint_2A:
1:	cli 
	pushal
	pushl $0x2A
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_2B:
__hwint_2B:
1:	cli 
	pushal
	pushl $0x2B
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_2C:
__hwint_2C:
1:	cli 
	pushal
	pushl $0x2C
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_2D:
__hwint_2D:
1:	cli 
	pushal
	pushl $0x2D
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_2E:
__hwint_2E:
1:	cli 
	pushal
	pushl $0x2E
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_2F:
__hwint_2F:
1:	cli 
	pushal
	pushl $0x2F
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_30:
__hwint_30:
1:	cli 
	pushal
	pushl $0x20
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_31:
__hwint_31:
1:	cli 
	pushal
	pushl $0x31
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_32:
__hwint_32:
1:	cli 
	pushal
	pushl $0x32
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_33:
__hwint_33:
1:	cli 
	pushal
	pushl $0x33
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_34:
__hwint_34:
1:	cli 
	pushal
	pushl $0x34
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_35:
__hwint_35:
1:	cli 
	pushal
	pushl $0x35
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_36:
__hwint_36:
1:	cli 
	pushal
	pushl $0x36
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_37:
__hwint_37:
1:	cli 
	pushal
	pushl $0x37
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_38:
__hwint_38:
1:	cli 
	pushal
	pushl $0x38
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_39:
__hwint_39:
1:	cli 
	pushal
	pushl $0x39
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_3A:
__hwint_3A:
1:	cli 
	pushal
	pushl $0x3A
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_3B:
__hwint_3B:
1:	cli 
	pushal
	pushl $0x3B
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_3C:
__hwint_3C:
1:	cli 
	pushal
	pushl $0x3C
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_3D:
__hwint_3D:
1:	cli 
	pushal
	pushl $0x3D
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_3E:
__hwint_3E:
1:	cli 
	pushal
	pushl $0x3E
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
_hwint_3F:
__hwint_3F:
1:	cli 
	pushal
	pushl $0x3F
	call int_dispatch
	addl  $0x04,%esp
	popal
	sti
	iret
	jmp 1b
	
	
