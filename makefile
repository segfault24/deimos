#!/bin/bash

ARCH=i386

ARCHDIR=arch/$(ARCH)

AR=/home/austin/opt/cross/bin/i686-elf-ar
CC=/home/austin/opt/cross/bin/i686-elf-gcc
CFLAGS=-ffreestanding -fbuiltin -Wall -Wextra -I$(CINC) -I$(KINC)
LDFLAGS=-nostdlib

# include directories
CINC=dlibc/include
KINC=kernel/include

#DLIBCOBJS
include dlibc/make.config
#ARCHOBJS
include $(ARCHDIR)/make.config
#KERNELOBJS
include kernel/make.config
#ALLOBJS
ALLOBJS=$(DLIBCOBJS) $(ARCHOBJS) $(KERNELOBJS)

# decoy (dont auto make everything)
decoy:

# compile dlibc
dlibc.a: $(DLIBCOBJS)
	$(AR) rcs $@ $(DLIBCOBJS)

# compile kernel
deimos.bin: $(ALLOBJS) dlibc.a $(ARCHDIR)/kernel.lnk
	$(CC) -T $(ARCHDIR)/kernel.lnk -o $@ $(ALLOBJS) dlibc.a $(CFLAGS) $(LDFLAGS)

# global compile rules
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.s
	$(CC) -c $< -o $@ $(CFLAGS)

# create a bootable iso
deimos.iso: deimos.bin
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$<
	echo menuentry "deimos" {	>> isodir/boot/grub/grub.cfg
	echo multiboot /boot/$<		>> isodir/boot/grub/grub.cfg
	echo }						>> isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir
	rm -rf isodir

# utility & housekeeping
all: dlibc.a deimos.bin

iso: deimos.iso

run:
	#qemu -cdrom demios.iso

clean:
	rm -f *.o */*.o */*/*.o dlibc.a deimos.bin deimos.iso

.PHONY: all iso run clean

