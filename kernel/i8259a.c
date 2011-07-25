/******************************************************************************
 * IFEM OS - kernel/i8259a.c                                                  * 
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
 * Every i386 based system uses 2 PICs (programmable interrupt controller) to
 * manage I/O devices. One chip is master and the other one is slave.
 * All the routines to get those chips working, are defined here.
 */
#include "kernel.h"

/* IC1 format:
     A7 | A6 | A5 | 1 | LTIM | ADI | SNGL | IC4
     
     - A7, A6, A5: not used in i386
     - LTIM: 1=Level triggered mode, 0=edge triggered mode
     - ADI: Call Address Interval; 0=interval of 8, 1=interval of 4
     - SNGL: 1=single, 0=cascaded
     - IC4: 1=ICW4 needed, 0=no ICW4 needed
 */
#define ICW1	0x11

/* IC2 format:
     A15/T7 | A14/T6 | A13/T5 | A12/T4 | A11/T3 | A10 | A9 | A8
     
     - A15..A8: A(15-8) of interrupt vector address (MCS-80/85 mode)
     - T7..T3: T(7-3) of interrupt vector address (8086/8088 mode)
 */
/* IC3 format (master device):
     S7 | S6 | S5 | S4 | S3 | S2 | S1 | S0
     
     - S7..S0: 1=IR input has a slave, 0=IR input does not have a slave
     
   IC3 format (slave device):
     0 | 0 | 0 | 0 | 0 | ID2 | ID1 | ID0
     
     - ID2..ID0: Slave ID
 */
/* IC4 format:
     0 | 0 | 0 | SFNM | BUF | M/S | AEOI | mPM
     
     - SFNM: 1=special fully nestedm mode, 0=not special fully nestedm mode
     - BUF | M/S: 
        0  |  X = none buffered mode
        1  |  0 = buffered mode/slave
        1  |  1 = buffered mode/master
     - AEOI: 1=auto EOI, 0=no auto EOI
     - mPM: 1=8086/8088 mode, 0=MCS-80/85 mode
 */
#define ICW4	0x01

PUBLIC void pic_init(void);
PUBLIC void irq_enable(int);
PUBLIC void irq_set_handler(int, irq_handler_t);

/*============================================================================*
 *				     pic_init				      *
 *============================================================================*/
void pic_init(void)
{
/* This function initializes the programmable interrupt controller chips. One is
   set to master mode and the other one to slave mode, cascaded through master.
   All interrupts should be disabled during the operation.
 */

  int i;
  
  irq_status = 0xFFFF  ^ (1 << IRQ_CASCADE);  /* all lines except the cascaded 
  						 one are disabled. */
  _cli();
  
  _outb(PIC1_CTRL, ICW1);  /* ICW1: init master */
  _outb(PIC1_CTRL_MASK, IRQ0_VECT);  /* ICW2: set base IRQ vector for master */
  _outb(PIC1_CTRL_MASK, (0x01 << IRQ_CASCADE));  /* ICW3: setting slave cascade
  						    line */
  _outb(PIC1_CTRL_MASK, ICW4);	/* ICW4: processor mode select */
  _outb(PIC1_CTRL_MASK, irq_status & 0xFF); /* initiate master IRQ lines */
  
  _outb(PIC2_CTRL, ICW1);  /* ICW1: init slave */
  _outb(PIC2_CTRL_MASK, IRQ8_VECT);  /* ICW2: set base IRQ vector for slave */
  _outb(PIC2_CTRL_MASK, IRQ_CASCADE);  /* ICW3: slave ID */
  _outb(PIC2_CTRL_MASK, ICW4);  /* ICW4: processor mode select */
  _outb(PIC2_CTRL_MASK, (char)(irq_status >> 8)); /* initiate slave IRQ lines */

  for (i=0; i<IRQ_NR_VECT; i++)
  	irq_table[i] = NULL;
}

/*============================================================================*
 *				   irq_enable				      *
 *============================================================================*/
PUBLIC void irq_enable(int irq)
{
/* Enable an IRQ line in one of the i8259a's and resets its bit in 'irq_status'.
   IRQ > 16 or < 0 causes panic.
 */
 
  _cli();	/* no interrupts allowed. maybe we should allow them? */
  
  if (irq < 0 || irq > 15)
  	panic("irq_enable(): Invalid IRQ.\0");

  irq_status &= (0xFF ^ (0x01 << irq));
  if (irq < 8)
  	_outb(PIC1_CTRL_MASK, (char)irq_status);
  else
  	_outb(PIC2_CTRL_MASK, (char)(irq_status >> 8));
	
}

/*============================================================================*
 *				irq_set_handler				      *
 *============================================================================*/
PUBLIC void irq_set_handler(int irq, irq_handler_t func)
{
/* Puts a pointer to the IRQ handler routine in 'irq_table'. 
   IRQ > 16 or < 0 causes panic, also if the handler has already been set, it 
   panics.
 */
 
  if (irq < 0 || irq > 15)
	panic("irq_set_handler(): Invalid IRQ.\0");
  else
  if (irq_table[irq] != NULL)
  	panic("irq_set_handler(): IRQ handler already set.\0");
	
  irq_table[irq] = func;
}

