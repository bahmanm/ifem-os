################################################################################
# IFEM OS - boot/bootblock.s                                                   #
# Copyright (C) 2008 Bahman Movaqar (bahman AT bahmanm.com)                    #
#                                                                              #
# This program is free software; you can redistribute it and/or                #
# modify it under the terms of the GNU General Public License                  #
# as published by the Free Software Foundation; either version 2               #
# of the License, or (at your option) any later version.                       #
#                                                                              #
# This program is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with this program; if not, write to the Free Software                  #
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,   #
# USA.                                                                         #
################################################################################

.include "segconst.s"

.global boot

	.org	0
	.code16
	.balign 0x08
boot:
	jmp	begin
startup_size:	.word	0	# size of startup code
schedpit_off:	.long	0	# schedpit module offset
new_seg:	.word	0xff0e	# bootloader new segment
new_off:	.word	0x0	# bootloader new offset
bootdev:	.byte	0	# boot device (in DL)
curr_sect:	.byte	2	# current sector # when reading startup
curr_head:	.byte	0	# current head
curr_track:	.byte	0	# current track

NEW_SEG = 0x9f00
STACK_SEG = 0x6f00

begin:
	# Initializing segments.
	movw	$0x7c0,	%ax
	movw	%ax, %ds
	movw	%ax, %es
        #
	movw	$0xff60, %ax
	movw	%ax, %ss
        #
	movw	$0xfff0, %sp

        # Save the boot device.
	movb	%dl, bootdev

	# Write a msg to the screen, and show a progress bar (by dots).
	movw	$intro_msg, %ax
	movw	$30, %cx
	call	write_str
	call	write_dot

	# Move bootloader to new_seg:new_off. DS is already set.
	# Source address.
	movw	$0x7c0, %ax
	movw	%ax, %ds
	subw	%si, %si
	# Destination address.
	movw	$NEW_SEG, %ax
	movw	%ax, %es
	subw	%di, %di
        # How many words to be moved?
	movw	$256, %cx
        #
	cld
	rep     movsw

	call	write_dot
	ljmp	$NEW_SEG, $relocate 	# jump to new place

relocate:
	# Relocate bootloader.
	movw	$STACK_SEG, %ax
	movw	%ax, %ss
	movw	$0xfff0, %sp

	movw	$NEW_SEG, %ax
	movw	%ax, %ds
	movw	%ax, %es

	# Update progress bar.
	call	write_dot

reset_dev:
	# Reset the boot device (ie. Floppy).
	movb	bootdev, %dl
	movb	$0, %ah
	int	$0x13
	# Check for errors.
	jc	reset_error
	call	write_dot	# update progress bar
	jmp	read_startup
reset_error:
	movw	$reseterr_msg, %ax
	movw	$14, %cx
	call	write_str
	jmp	hlt_on_err

read_startup:
	# Read the secondary boot code.
	movw	startup_size, %cx
	movw	$K_SEG, %ax	# buffer segment
	movw	%ax, %es
	movw	$K_OFF, %bx	# buffer offset
read_loop:
	pushw	%cx
	cmpw	$0, %cx		# all the startup has been read?
	jz	read_done
check_sect:
	# Have we read all the sectors in this disk face?
	cmpb	$19, curr_sect	# a 1.44 floppy has 18 sectors/track
	jne	check_head
	movb	$1, curr_sect
	incb	curr_head	# proceed to the next face
check_head:
	# Have we read all the sectors in the this track?
	cmpb	$2, curr_head
	jne	do_read
	movb	$0, curr_head
	incb	curr_track	# proceed to the next track
do_read:
	movb	$1, %al		# no of sectors to be read
	movb	curr_track, %ch
	movb	curr_sect, %cl
	movb	curr_head, %dh
	movb	bootdev, %dl
	movb	$2, %ah		# disk read service
	int	$0x13
	# Check for errors.
	jc	read_error
	call 	write_dot	# update progress bar
	# Update variables.
	movw	%es, %ax	# advance 512 bytes in destination segment
	addw	$0x20, %ax
	movw	%ax, %es
	incb	curr_sect	# next sector
	popw	%cx		# decrease total # of sectors to be read
	decw	%cx
	jmp	read_loop
read_done:
	# Startup code loaded successfully. Turn off drive motor and Jump to it.
	# Taken from NUXI.
        movw    $0x03f2, %dx       # turn off drive motor
        xorb    %al, %al
        outb    %al, %dx
	ljmp	$K_SEG, $K_OFF

read_error:
	# An error occured reading from the disk.
	#call	find_cursor
	movw	$readerr_msg, %ax
	movw	$13, %cx
	call 	write_str
	jmp	hlt_on_err

hlt_on_err:
	# Loop forever, awaiting CTRL-ALT-DEL.
	jmp	hlt_on_err

	#######################################################################
	#                              write_str                              #
	#######################################################################
write_str:
	# Write a string to the screen. The string segment is in ES and offset
	# is in AX. The position in row and column is in DH & DL respectively.
	# The length of the string is in CX.
	#
	movw	%ax, %bx	# string offset
	movb	$0x0d, %bl	# output color
	movb	$0x0e, %ah	# service #
write_loop:
	movb	(%bx), %al
	pushw	%bx
	int	$0x10
	popw	%bx
	incw	%bx
	loop	write_loop

	ret

	#######################################################################
	#                             write_dot                               #
	#######################################################################
write_dot:
	# Write a '.' to the screen.
	#
	movb	$('.'), %al
	movb	$0x0e, %ah
	int	$0x10

	ret

	#######################################################################
	# Data storage
intro_msg:	.ascii	"IFEM Boot Loader 0.1"
		.byte	0x0d, 0x0a
		.ascii	"Loading"
reseterr_msg:	.byte	0x0d, 0x0a
		.ascii	"Reset error!"
readerr_msg:	.byte	0x0d, 0x0a
		.ascii	"Read error!"

	.org	510
	.byte	0x55, 0xaa
