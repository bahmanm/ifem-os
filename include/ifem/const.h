/******************************************************************************
 * IFEM OS - include/ifem/const.h                                             * 
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
 * Defines some useful constants and macros used by all parts of the system.
 */
#ifndef IFEM__CONST_H
#define IFEM__CONST_H

/* These macros are mostly used for prototyping functions and sometimes
   variables.
 */
#define PRIVATE		static
#define PUBLIC
		
/* The EXTERN macro, anywhere, expands to 'extern' except 'varalloc.c' in which
   it expands to nothing, causing memory room being allocated for all EXTERN
   variables. That's a clever tirck taken from MINIX.
 */
#ifndef _VARALLOC
#define EXTERN		extern
#else
#undef  EXTERN
#define EXTERN
#endif

#ifdef  NULL
#undef  NULL
#endif
#define NULL	((void *)(0))

#define FREQ	60	/* timer frequency */
#define TICKS2CALL	300	/* how many clock ticks to ignore, prior to 
				   calling scheduler */
				   
#endif /* IFEM__CONST_H */
