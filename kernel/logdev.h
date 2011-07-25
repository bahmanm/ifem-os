/******************************************************************************
 * IFEM OS - kernel/logdev.h                                                  * 
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
 * Defines a priliminary log device for kernel. This can be any device (e.g.
 * Console or Printer.). All that is needed is to implement an interface to
 * the device which is able to support the following routines:
 *   - write a character to the device
 *   - write a string to the device (could be implemented using above)
 *   - reset the device
 */
#ifndef KERNEL__LOGDEV_H
#define KERNEL__LOGDEV_H

/* Some common log devices. */
#define LOGDEV_SYSCON	0	/* system console */
#define LOGDEV_PRINTER	1	/* printer */

/* The kernel log device. */
#define KLOGDEV		LOGDEV_SYSCON	/* much easier than anything! */

/* Variables which are only visible to this file and 'kernel/logdev.c'. */
#if (KLOGDEV == LOGDEV_SYSCON)
#define LOG_MAX_X	24	  /* x and y boundaries on the PC console */
#define LOG_MAX_Y	79
#define log_offset	(pos_x * (LOG_MAX_Y+1) + pos_y)  /* Offset from the 
							    console memory 
							    beginning. */
PRIVATE unsigned short *con_org;  /* beginning of console memory */
PRIVATE int pos_x;		  /* position of cursor on the screen */
PRIVATE int pos_y; 
PRIVATE unsigned char attr;  	  /* character attribute */
#endif

#endif /* KERNEL__LOGDEV_H */
