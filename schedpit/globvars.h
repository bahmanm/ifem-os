/******************************************************************************
 * IFEM OS - schedpit/globvars.h                                              * 
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
 * Declares all global variable which are used within schedpit module. Space
 * for them is reserved in 'schedpit/varalloc.c'.
 */
#ifndef SCHEDPIT__GLOBVARS_H
#define SCHEDPIT__GLOBVARS_H

EXTERN struct tss_s schedpit_tss;  /* 'schedpit' TSS */
EXTERN u8_t schedpit_stack[SCHEDPIT_STACK_SIZE];  /* stack space for schedpit */

#endif /* SCHEDPIT__GLOBVARS_H */
