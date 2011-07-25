/******************************************************************************
 * IFEM OS - kernel/logdev.c                                                  * 
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
 * Implements a priliminary interface to kernel log device. This interface
 * should at least have the following routines:
 *   - log_char
 *   - log_str
 *   - log_clr
 *   - log_init
 */
#include "kernel.h"
#include "logdev.h"

/* Device interface */
PUBLIC void log_char(char);
PUBLIC void log_str(char[]);
PUBLIC void log_num(unsigned short);
PUBLIC void log_clr(void);
PUBLIC void log_init(void);
PUBLIC void change_attr(u8_t);

/*============================================================================*
 *				    log_char				      *
 *============================================================================*/
void log_char(char ch)
{
/* Write a character to kernel log device. */

#if (KLOGDEV == LOGDEV_SYSCON)
  register unsigned short tmp;
  if (ch == '\n') {
  	pos_x++; 
	pos_y = 0;
  } else {  
  	tmp = attr;
  	tmp = (tmp << 8) + ch;
  
  	*(con_org + log_offset) = tmp;
  	pos_y++;
  }
  
  if (pos_x > LOG_MAX_X) {
  	log_clr();
  	pos_x = pos_y = 0;
  } else
  if (pos_y == LOG_MAX_Y) {
	pos_x++;
	pos_y = 0;
  }
#endif
}

/*============================================================================*
 *				   log_str				      *
 *============================================================================*/
void log_str(char *str)
{
/* Writes a string to kernel log device. The string should be a null terminated
   one.
 */
 
  register int i;
  
  for (i=0; str[i]!='\0'; i++)
  	log_char(str[i]);
}

/*============================================================================*
 *				    log_num				      *
 *============================================================================*/
void log_num(unsigned short num)
{
/* Converts a hexadecimal 16-bit word to string and writes it to the screen. */

  char nstr[7] = "0x";
  register unsigned short n;
  
  nstr[6] = '\0';
  nstr[5] = (n=num&0x000F) < 0xA ? n+'0' : n+'A'-0xA;
  nstr[4] = (n=(num&0x00F0) >> 4) < 0xA ? n+'0' : n+'A'-0xA;
  nstr[3] = (n=(num&0x0F00) >> 8) < 0xA ? n+'0' : n+'A'-0xA;
  nstr[2] = (n=(num&0xF000) >> 12) < 0xA ? n+'0' : n+'A'-0xA;

  log_str(nstr);
}

/*============================================================================*
 *				    log_clr				      *
 *============================================================================*/
void log_clr(void)
{
/* Clears kernel log device. */
  
#if (KLOGDEV == LOGDEV_SYSCON)
  register int i;
#define BLANK	0x0720	/* forecolor: black, backcolor: white */
  
  for (i=0; i<((LOG_MAX_X+1) * (LOG_MAX_Y+1)); i++)
  	*(con_org + i) = BLANK;
  pos_x = pos_y = 0;
  
#undef BLANK
#endif
}

/*============================================================================*
 *				   log_init				      *
 *============================================================================*/
void log_init(void)
{
/* Initializes kernel log device. */
  
  /* Initialize the private variables. */
#if (KLOGDEV == LOGDEV_SYSCON)
  con_org = (unsigned short *)0xB8000;
  pos_x = 0;
  pos_y = 0;
  attr = 0x0F;	/* high nibble: backcolor(red), low nibble: forecolor(yellow) */
#endif
  log_clr();
}

/*============================================================================*
 *				   change_attr				      *
 *============================================================================*/
PUBLIC void change_attr(u8_t val)
{
/* Change characters atrribute, i.e. fore and back color. */

  attr = val;	/* that's all folks! */
}
