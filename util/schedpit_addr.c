/******************************************************************************
 * IFEM OS - util/schedpit_addr.c                                             * 
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
 * Compute the 'schedpit' starting address in memory as below:
 *   schedpit address = kernel size + 0x600
 * and write it to a file as:
 *   SCHEDPIT_ADDR=	schedpit address
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define err_exit(retcode)						     \
	  { fprintf(stderr, "(schedpit_addr) Error: %s\n", strerror(errno)); \
	    exit((retcode)); }

int main(int argc, char *argv[])
{
	int fd;
	long addr, kernel_size;
	char str[30];

	if (argc == 1) {
		fprintf(stderr, "Usage: schedpit_addr IFEM_BIN destfile\n");
		return 0;
	}

	if ((fd = open(argv[1], O_RDWR)) < 0) err_exit(1);
	
	if ((kernel_size = lseek(fd,0,SEEK_END)) < 0) err_exit(1);
	addr = kernel_size + 0x600;
	close(fd);
	
	sprintf(str, "SCHEDPIT_ADDR=\t0x%X\n#", addr, '\0');
	if ((fd = open(argv[2], O_WRONLY | O_CREAT)) < 0) err_exit(1); 
	if (write(fd, str, strlen(str)) <= 0) err_exit(1);
	close(fd);
	
	fprintf(stderr, "=====================\n");
	fprintf(stderr, "schedpit_addr Report:\n");
	fprintf(stderr, "=====================\n"); 
	fprintf(stderr, "ifem-os.bin size = %ld bytes\n", kernel_size);
	fprintf(stderr, "Kernel image successfully modified!\n");
	
	return 0;
}
