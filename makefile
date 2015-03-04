#!/bin/bash

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
_all: all

# compile kernel
deimos.bin: $(ARCHOBJS) $(KERNELOBJS) $(ARCHDIR)/kernel.ld
	$(CC) -T $(ARCHDIR)/kernel.ld -o $@ $(ARCHOBJS) $(KERNELOBJS) $(CFLAGS) $(LDFLAGS)

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
all: kernel iso drivers
kernel: deimos.bin
iso: deimos.iso
drivers: $(DRIVEROBJS)

run: iso
	qemu -cdrom deimos.iso -monitor stdio -net nic,model=rtl8139

clean:
	rm -f *.o */*.o */*/*.o deimos.bin deimos.iso

.PHONY: all kernel iso run clean

