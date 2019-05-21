/******************************************************************************
 * IFEM OS - kernel/globfunc.h                                                *
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
 * Prototypes of all global function used kernel-wide.
 */
#ifndef KERNEL__GLOBFUNC_H
#define KERNEL__GLOBFUNC_H

/*
 * asmlib.S
 */
PUBLIC void _sti(void);	 /* enables interrupts */
PUBLIC void _cli(void);  /* diables interrupts */
PUBLIC void _reload_gdt(void);  /* reloads gdt to GDTR */
PUBLIC void _reload_idt(void);  /* reloads idt to IDTR */
PUBLIC void _save_idt();	/* saves IDTR to idt */
PUBLIC void _outb(port_t, u8_t);  /* outputs one byte to an I/O port */
PUBLIC u16_t _inb(port_t port); /* inputs a byte from an I/O port */
PUBLIC void _task_run(unsigned);  /* switches to kernel task */
PUBLIC u16_t _ts_flag(void);	/* returns TS (task switched) flag */
PUBLIC void _reset_busy(u16_t);  /* reset busy bit of a TSS descriptor */

/*
 * logdev.c
 */
PUBLIC void log_char(char);  /* write a char to kernel log device */
PUBLIC void log_str(char[]);          /* write a str to kernel log device */
PUBLIC void log_num(unsigned short);
PUBLIC void log_clr(void);	      /* clear kernel log device */
PUBLIC void log_init(void);	      /* initialize variables and clear dev */
PUBLIC void change_attr(u8_t);	      /* change character attribute */

/*
 * int.c
 */
PUBLIC void hwint_init(void);	/* init hardware interrupt vectors */

/*
 * isr.S
 */
PUBLIC void _hwint_00(void);
PUBLIC void _hwint_01(void);
PUBLIC void _hwint_02(void);
PUBLIC void _hwint_06(void);
PUBLIC void _hwint_03(void);
PUBLIC void _hwint_04(void);
PUBLIC void _hwint_05(void);
PUBLIC void _hwint_07(void);
PUBLIC void _hwint_08(void);
PUBLIC void _hwint_09(void);
PUBLIC void _hwint_0A(void);
PUBLIC void _hwint_0B(void);
PUBLIC void _hwint_0C(void);
PUBLIC void _hwint_0D(void);
PUBLIC void _hwint_0E(void);
PUBLIC void _hwint_0F(void);

PUBLIC void _hwint_10(void);
PUBLIC void _hwint_11(void);
PUBLIC void _hwint_12(void);
PUBLIC void _hwint_13(void);
PUBLIC void _hwint_14(void);
PUBLIC void _hwint_15(void);
PUBLIC void _hwint_16(void);
PUBLIC void _hwint_17(void);
PUBLIC void _hwint_18(void);
PUBLIC void _hwint_19(void);
PUBLIC void _hwint_1A(void);
PUBLIC void _hwint_1B(void);
PUBLIC void _hwint_1C(void);
PUBLIC void _hwint_1D(void);
PUBLIC void _hwint_1E(void);
PUBLIC void _hwint_1F(void);

/* Master chip IRQs. */
PUBLIC void _hwint_20(void);	/* irq 0: clock */
PUBLIC void _hwint_21(void);	/* irq 1: keyboard */
PUBLIC void _hwint_22(void);	/* irq 2: cascade */
PUBLIC void _hwint_23(void);	/* irq 3: 2nd serial */
PUBLIC void _hwint_24(void);	/* irq 4: 1st serial */
PUBLIC void _hwint_25(void);	/* irq 5: XT winchester */
PUBLIC void _hwint_26(void);	/* irq 6: floppy */
PUBLIC void _hwint_27(void);	/* irq 7: printer */
/* Slave chip IRQs. */
PUBLIC void _hwint_28(void);	/* irq 8: real time clock */
PUBLIC void _hwint_29(void);	/* irq 9: irq2 redirected */
PUBLIC void _hwint_2A(void);	/* irq 10 */
PUBLIC void _hwint_2B(void);	/* irq 11 */
PUBLIC void _hwint_2C(void);	/* irq 12 */
PUBLIC void _hwint_2D(void);	/* irq 13: FPU exception */
PUBLIC void _hwint_2E(void);	/* irq 14: AT winchester */
PUBLIC void _hwint_2F(void);	/* irq 15 */

PUBLIC void _hwint_30(void);
PUBLIC void _hwint_31(void);
PUBLIC void _hwint_32(void);
PUBLIC void _hwint_33(void);
PUBLIC void _hwint_34(void);
PUBLIC void _hwint_35(void);
PUBLIC void _hwint_36(void);
PUBLIC void _hwint_37(void);
PUBLIC void _hwint_38(void);
PUBLIC void _hwint_39(void);
PUBLIC void _hwint_3A(void);
PUBLIC void _hwint_3B(void);
PUBLIC void _hwint_3C(void);
PUBLIC void _hwint_3D(void);
PUBLIC void _hwint_3E(void);
PUBLIC void _hwint_3F(void);

PUBLIC int int_dispatch(unsigned);

/*
 * cpu.c
 */
/* Add descriptors at the requested index to IDT and GDT. */
PUBLIC unsigned add2gdt(struct segdesc_s *, unsigned short);
PUBLIC unsigned add2idt(struct gatdesc_s *, unsigned short);
/* Setup descriptors. */
PUBLIC void setup_tss_desc(struct tssdesc_s *, struct tss_s *, u8_t, u8_t);
/* Setup gates. */
PUBLIC void setup_task_gate(struct gatdesc_s *, u16_t, u8_t);
PUBLIC void setup_call_gate(struct gatdesc_s *, u32_t, u16_t, u8_t, u8_t);
PUBLIC void setup_int_gate(struct gatdesc_s *, u32_t, u16_t, u8_t);
/* Builds up a TSS. */
PUBLIC void setup_tss(struct tss_s *, task_entry_t, u16_t, u16_t, reg_t, reg_t,
		      u16_t, u16_t);

/*
 * i8259a.c
 */
PUBLIC void pic_init(void);
PUBLIC void irq_enable(int);
PUBLIC void irq_set_handler(int, irq_handler_t);

/*
 * irqhandl.c
 */
/* Master i8259a chip IRQs: */
PUBLIC int irq_00(int);        /* int 0x20, irq 0: clock */
PUBLIC int irq_01(int);        /* int 0x21, irq 1: keyboard */
PUBLIC int irq_02(int);        /* int 0x22, irq 2: cascade */
PUBLIC int irq_03(int);        /* int 0x23, irq 3: 2nd serial */
PUBLIC int irq_04(int);       /* int 0x24, irq 4: 1st serial */
PUBLIC int irq_05(int);       /* int 0x25, irq 5: XT winchester */
PUBLIC int irq_06(int);       /* int 0x26, irq 6: floppy */
PUBLIC int irq_07(int);       /* int 0x27, irq 7: printer */
/* Slave i8259a chip IRQs: */
PUBLIC int irq_08(int);       /* int 0x28, irq 8: real time clock */
PUBLIC int irq_09(int);       /* int 0x29, irq 9: irq2 redirected */
PUBLIC int irq_10(int);       /* int 0x2A, irq 10 */
PUBLIC int irq_11(int);       /* int 0x2B, irq 11 */
PUBLIC int irq_12(int);       /* int 0x2C, irq 12 */
PUBLIC int irq_13(int);       /* int 0x2D, irq 13: FPU exception */
PUBLIC int irq_14(int);       /* int 0x2E, irq 14: AT winchester */
PUBLIC int irq_15(int);       /* int 0x2F, irq 15 */

/*
 * main.c
 */
PUBLIC void panic(char *);
PUBLIC void log_gdt(void);

/*
 * intrface.c
 */
PUBLIC void i_add2gdt(u32_t, struct segdesc_s *, unsigned short, u32_t);
PUBLIC void i_add2idt(u32_t, struct gatdesc_s *, unsigned short, u32_t);

#endif
