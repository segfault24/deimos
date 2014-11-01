#!/bin/bash

ARCH=i386

ARCHDIR=arch/$(ARCH)

# compiler and linker locations & options
AR=/home/austin/opt/cross/bin/i686-elf-ar
CC=/home/austin/opt/cross/bin/i686-elf-gcc
CFLAGS=-g -ffreestanding -fbuiltin -Wall -Werror -Wextra -I$(CINC) -I$(KINC) -I$(AINC)
LDFLAGS=-nostdlib -lgcc

# include directories
CINC=dlibc/include
AINC=$(ARCHDIR)/include
KINC=kernel/include

#DLIBCOBJS
include dlibc/make.config
#ARCHOBJS
include $(ARCHDIR)/make.config
#KERNELOBJS
include kernel/make.config
#OSOBJS
OSOBJS=$(ARCHOBJS) $(KERNELOBJS)

# default target (see below)
_all: all

# compile dlibc
dlibc.a: $(DLIBCOBJS)
	$(AR) rcs $@ $(DLIBCOBJS)

# compile kernel
deimos.bin: $(OSOBJS) dlibc.a $(ARCHDIR)/kernel.lnk
	$(CC) -T $(ARCHDIR)/kernel.lnk -o $@ $(OSOBJS) dlibc.a $(CFLAGS) $(LDFLAGS)

# generic compile rules
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.s
	$(CC) -c $< -o $@ $(CFLAGS)

# create a bootable iso
deimos.iso: deimos.bin
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$<
	echo menuentry "deimos" { >> isodir/boot/grub/grub.cfg
	echo multiboot /boot/$< >> isodir/boot/grub/grub.cfg
	echo } >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir
	rm -rf isodir

# actual targets
all: dlibc kernel
dlibc: dlibc.a
kernel: deimos.bin
iso: deimos.iso

run:
	#qemu -cdrom demios.iso

clean:
	rm -f *.o */*.o */*/*.o dlibc.a deimos.bin deimos.iso

.PHONY: all iso run clean

