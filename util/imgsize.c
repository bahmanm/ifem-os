/******************************************************************************
 * IFEM OS - util/imgsize.c                                                   *
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
 * Writes the IFEM boot image size in sectors to the bootloader.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define err_exit(retcode)						     \
		{ fprintf(stderr, "(imgsize) Error: %s\n", strerror(errno)); \
		  exit((retcode)); }

int main(int argc, char *argv[])
{
	int fd, fd2;
	unsigned short sectors, dummy;
	long size, addr1, padd_size, i;
	unsigned char ch;

	if (argc != 3) {
		fprintf(stderr, "Usage: imgsize IFEM_IMG KERNEL_IMG\n");
		return 0;
	}

	if ((fd = open(argv[1], O_RDWR)) < 0) err_exit(1);

	if ((fd2 = open(argv[2], O_RDWR)) < 0) err_exit(1);
	if ((addr1 = lseek(fd2,0,SEEK_END)) < 0) err_exit(1);
	addr1 += 0x600;
	close(fd2);

	if ((size = lseek(fd,0,SEEK_END)) < 0) err_exit(1);
	sectors = (size - 512) / 512 + 1;

	if (lseek(fd, 2, SEEK_SET) < 0) err_exit(1);
	if (write(fd, (char *)&sectors, sizeof(sectors)) <= 0) err_exit(1);
	if (write(fd, (char *)&addr1, sizeof(addr1)) <= 0) err_exit(1);

	padd_size = size % 512;
	if (lseek(fd, 0, SEEK_END) < 0) err_exit(1);
	for (ch=0, i=padd_size; i>0; i--)
		if (write(fd, &ch, 1) < 0) err_exit(1);

	close(fd);

	fprintf(stderr, "===============\n");
	fprintf(stderr, "imgsize report:\n");
	fprintf(stderr, "===============\n");
	fprintf(stderr, "Kernel image size = %ld bytes\n", size);
	fprintf(stderr, "Kernel image size = %d sectors\n", sectors);
	fprintf(stderr, "Padding size      = %ld bytes\n", padd_size);
	fprintf(stderr, "Kernel image successfully modified!\n");

	return 0;
}
