/******************************************************************************
 * IFEM OS - kernel/int.c                                                     * 
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
 * Contains hardware interrupt routines.
 * In a i386 system, 8259 interrupt controller is used which supports 8 IRQs. 
 * Typically two chips are used in master-slave manner. The master support 7
 * IRQs of its 8 limit and puts the remainng IRQ for cascading slave chip
 * interrupts to CPU. This way an interrupt in slave should first pass the
 * master through that IRQ and that is the master who informs the CPU about
 * the interrupt.
 * Routines are named like this: hwint_nn, where nn is the IRQ number the
 * routine is going to handle.
 */
#include "kernel.h"

#define setup_gate(func, irq) 						       \
	   setup_int_gate(&gate, (u32_t)(func), GDT_KERNEL_CODE, 0x8E);        \
	   add2idt(&gate, (irq));		
	   		
PUBLIC void hwint_init(void);	/* inits hardware interrupt vectors */
PUBLIC int int_dispatch(unsigned int_no);  /* dispatches interrupts to proper
					      handlers */

/*============================================================================*
 *				  init_hwint				      *
 *============================================================================*/
void hwint_init(void)
{
/* Initialize hardware interrupt and exception vectors. */
	
  struct gatdesc_s gate;

  /* Exceptions: */
  setup_gate(_hwint_00, 0x00);	/* int 0x00: devide error */
  setup_gate(_hwint_01, 0x01);	/* int 0x01: debug exceptions */  
  setup_gate(_hwint_02, 0x02);	
  setup_gate(_hwint_06, 0x06);	/* int 0x03: breakpoint */
  setup_gate(_hwint_03, 0x03);	/* int 0x04: overflow */
  setup_gate(_hwint_04, 0x04);	/* int 0x05: bounds check */
  setup_gate(_hwint_05, 0x05);	/* int 0x06: invalid opcode */
  setup_gate(_hwint_07, 0x07);	/* int 0x07: coporocessor not available */
  setup_gate(_hwint_08, 0x08);	/* int 0x08: double fault */
  setup_gate(_hwint_09, 0x09);	/* int 0x09: coporocessor segment overrun */
  setup_gate(_hwint_0A, 0x0A);	/* int 0x0A: invalid TSS */
  setup_gate(_hwint_0B, 0x0B);	/* int 0x0B: segment not present */
  setup_gate(_hwint_0C, 0x0C);	/* int 0x0C: stack exception */
  setup_gate(_hwint_0D, 0x0D);	/* int 0x0D: general protection exception */
  setup_gate(_hwint_0E, 0x0E);	/* int 0x0E: page fault */
  setup_gate(_hwint_0F, 0x0F);	
  
  setup_gate(_hwint_10, 0x10);	/* int 0x10: coprocessor error */
  setup_gate(_hwint_11, 0x11);
  setup_gate(_hwint_12, 0x12);
  setup_gate(_hwint_13, 0x13);
  setup_gate(_hwint_14, 0x14);
  setup_gate(_hwint_15, 0x15);
  setup_gate(_hwint_16, 0x16);
  setup_gate(_hwint_17, 0x17);
  setup_gate(_hwint_18, 0x18);
  setup_gate(_hwint_19, 0x19);
  setup_gate(_hwint_1A, 0x1A);
  setup_gate(_hwint_1B, 0x1B);
  setup_gate(_hwint_1C, 0x1C);
  setup_gate(_hwint_1D, 0x1D);
  setup_gate(_hwint_1E, 0x1E);
  setup_gate(_hwint_1F, 0x1F); 
  
  /* Master chip IRQs. */
  setup_gate(_hwint_20, 0x20);  	/* irq 0: clock */
  setup_gate(_hwint_21, 0x21);  	/* irq 1: keyboard */
  setup_gate(_hwint_22, 0x22);  	/* irq 2: cascade */
  setup_gate(_hwint_23, 0x23);  	/* irq 3: 2nd serial */
  setup_gate(_hwint_24, 0x24);  	/* irq 4: 1st serial */
  setup_gate(_hwint_25, 0x25);  	/* irq 5: XT winchester */
  setup_gate(_hwint_26, 0x26);  	/* irq 6: floppy */
  setup_gate(_hwint_27, 0x27);  	/* irq 7: printer */
  /* Slave chip IRQs. */
  setup_gate(_hwint_28, 0x28);  	/* irq 8: real time clock */
  setup_gate(_hwint_29, 0x29);  	/* irq 9: irq2 redirected */
  setup_gate(_hwint_20, 0x2A);  	/* irq 10 */
  setup_gate(_hwint_21, 0x2B);  	/* irq 11 */
  setup_gate(_hwint_22, 0x2C);  	/* irq 12 */
  setup_gate(_hwint_23, 0x2D);  	/* irq 13: FPU exception */
  setup_gate(_hwint_24, 0x2E);  	/* irq 14: AT winchester */
  setup_gate(_hwint_25, 0x2F);  	/* irq 15 */
  
  setup_gate(_hwint_30, 0x30);
  setup_gate(_hwint_31, 0x31);
  setup_gate(_hwint_32, 0x32);
  setup_gate(_hwint_33, 0x33);
  setup_gate(_hwint_34, 0x34);
  setup_gate(_hwint_35, 0x35);
  setup_gate(_hwint_36, 0x36);
  setup_gate(_hwint_37, 0x37);
  setup_gate(_hwint_38, 0x38);
  setup_gate(_hwint_39, 0x39);
  setup_gate(_hwint_3A, 0x3A);
  setup_gate(_hwint_3B, 0x3B);
  setup_gate(_hwint_3C, 0x3C);
  setup_gate(_hwint_3D, 0x3D);
  setup_gate(_hwint_3E, 0x3E);
  setup_gate(_hwint_3F, 0x3F);  	
}

/*============================================================================*
 *				    irq_noinit				      *
 *============================================================================*/
int int_dispatch(unsigned int_no)
{
/* This is used to initialize interrupt handlers before any tasks have 
   registered and after registeration calling the proper handlers.
 */
 
  if (int_no <= 0x10) {
  	log_str("\nint_dispatch(): Unhandled Exception \0");
	log_num(int_no);
	log_char('\n');
	return 1;
  }
  
  if (int_no < 0x20 || int_no > 0x2F) {
	log_str("\nint_dispatch(): Unhandled INT \0");
	log_num(int_no);
	log_char('\n');
	return 1;
  } 
  
  if (int_no >= 0x20 && int_no <= 0x2F) {
  	if (irq_table[int_no - 0x20] == NULL) {
		log_str("\nint_dispatch(): Unhandled IRQ \0");
		log_num(int_no - 0x20);
		log_char('\n');
		return 1;
  	} else 
		return irq_table[int_no-0x20](int_no-0x20);
  }
  
  log_str("\nint_dispatch(): Suspicious INT \0");
  log_num(int_no);
  log_char('\n');
  return 1;
}
