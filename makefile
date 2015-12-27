#!/bin/bash
#test123

ARCH=i386
ARCHDIR=arch/$(ARCH)

# compiler and linker locations & options
CC=/home/austin/opt/cross/bin/i686-elf-gcc
CFLAGS=-g -ffreestanding -fno-builtin -Wall -Werror -Wextra -Iinclude
LDFLAGS=-nostdlib -lgcc

#ARCHOBJS
include $(ARCHDIR)/make.config
#KERNELOBJS
include kernel/make.config
#DRIVERS
include drivers/make.config

# dummy default target (see below)
_all: default

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
default: kernel drivers
kernel: deimos.bin
drivers: $(DRIVEROBJS)

iso: kernel
	mkdir -p isodir/boot/grub
	cp deimos.bin isodir/boot/deimos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o deimos.iso isodir
	rm -rf isodir

run: kernel
	qemu-system-i386 -kernel deimos.bin -monitor stdio -device rtl8139
	#qemu-system-i386 -cdrom deimos.iso -monitor stdio -device rtl8139

pxe: kernel
	rm -rf ~/tftp_dir/*
	grub-mknetdir --net-directory=/home/austin/tftp_dir/
	cp deimos.bin ~/tftp_dir/boot
	cp grub.cfg ~/tftp_dir/boot/grub/
	chmod -R 777 ~/tftp_dir/*

clean:
	rm -rfv ~/tftp_dir/*
	rm -fv *.o */*.o */*/*.o deimos.bin deimos.iso

.PHONY: all kernel iso run clean

