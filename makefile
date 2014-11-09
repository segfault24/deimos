#!/bin/bash

ARCH=i386
ARCHDIR=arch/$(ARCH)

# compiler and linker locations & options
AR=/home/austin/opt/cross/bin/i686-elf-ar
CC=/home/austin/opt/cross/bin/i686-elf-gcc
CFLAGS=-g -ffreestanding -fbuiltin -Wall -Werror -Wextra -I$(KINC) -I$(AINC)
LDFLAGS=-nostdlib -lgcc

# include directories
AINC=$(ARCHDIR)/include
KINC=kernel/include

#ARCHOBJS
include $(ARCHDIR)/make.config
#KERNELOBJS
include kernel/make.config
#OSOBJS
OSOBJS=$(ARCHOBJS) $(KERNELOBJS)

# dummy default target (see below)
_all: all

# compile kernel
deimos.bin: $(OSOBJS) $(ARCHDIR)/kernel.lnk
	$(CC) -T $(ARCHDIR)/kernel.lnk -o $@ $(OSOBJS) $(CFLAGS) $(LDFLAGS)

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
all: kernel iso
kernel: deimos.bin
iso: deimos.iso

run: iso
	qemu -cdrom deimos.iso

clean:
	rm -f *.o */*.o */*/*.o deimos.bin deimos.iso

.PHONY: all kernel iso run clean

