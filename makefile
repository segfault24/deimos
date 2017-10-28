#!/bin/sh

ARCH=i386
ARCHDIR=arch/$(ARCH)
GCCDIR=$(HOME)/cross

# compiler and linker locations & options
CC=$(GCCDIR)/bin/i686-elf-gcc
CFLAGS=-g -ffreestanding -fno-builtin -Wall -Werror -Wextra -Iinclude
LDFLAGS=-nostdlib -lgcc

#ARCHOBJS
include $(ARCHDIR)/make.config
#KERNELOBJS
include kernel/make.config
#DRIVERS
include drivers/make.config

# default target
all: kernel drivers

########################
# generic compile rules
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.s
	$(CC) -c $< -o $@ $(CFLAGS)

# compile kernel
deimos.bin: $(ARCHOBJS) $(KERNELOBJS) $(DRIVEROBJS) $(ARCHDIR)/kernel.ld
	$(CC) -T $(ARCHDIR)/kernel.ld -o $@ $(ARCHOBJS) $(KERNELOBJS) $(DRIVEROBJS) $(CFLAGS) $(LDFLAGS)

########################
# actual targets
kernel: deimos.bin
drivers: $(DRIVEROBJS)

iso: kernel
	mkdir -p isodir/boot/grub
	cp deimos.bin isodir/boot/deimos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o deimos.iso isodir

run: kernel
	qemu-system-i386 -kernel deimos.bin -monitor stdio -device rtl8139

pxe: kernel
	rm -rf tftpdir/*
	mkdir -p tftpdir
	grub-mknetdir --net-directory=tftpdir
	cp deimos.bin tftpdir/boot/
	cp grub.cfg tftpdir/boot/grub/
	chmod -R 777 tftpdir/*

clean:
	rm -rfv tftpdir isodir
	rm -fv deimos.bin deimos.iso
	find . -type f -name '*.o' -delete

.PHONY: all kernel iso run pxe clean

