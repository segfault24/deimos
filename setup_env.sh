#!/bin/sh

### built for debian 9.2.1
### your mileage my vary

### binutils version ###
export BVER=2.27
### gcc version ###
export GVER=4.9.4
### build working directory ###
export BUILDTEMP="$HOME/build-cross"
### cross compiler output location ##
export PREFIX="$HOME/cross"

### install everything ###
sudo apt-get install \
linux-headers-amd64 gcc g++ gcc-multilib make gdb valgrind \
git bison flex libgmp-dev libmpfr-dev libmpc-dev qemu grub \
xorriso tftpd-hpa tftp-hpa udhcpd

### grab binutils and gcc source ###
mkdir -p $PREFIX
mkdir -p $BUILDTEMP
cd $BUILDTEMP
wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BVER.tar.gz
wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GVER/gcc-$GVER.tar.gz
tar xvfz binutils-$BVER.tar.gz
tar xvfz gcc-$GVER.tar.gz

### set environment variables ###
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

### build binutils ###
mkdir -p $BUILDTEMP/build-binutils
cd $BUILDTEMP/build-binutils
../binutils-$BVER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

### build gcc ###
mkdir -p $BUILDTEMP/build-gcc
cd $BUILDTEMP/build-gcc
../gcc-$GVER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

### clean up ###
# rm -rf $BUILDTEMP


######################
# setup dhcp and tftp
######################
#  /etc/udhcpd.conf
#    siaddr     192.168.1.43
#    sname      eos
#    boot_file  /boot/grub/i386-pc/core.0
#  /etc/default/udhcpd
#    DHCPD_ENABLED="yes"
#  /etc/default/tftpd-hpa
#    TFTP_DIRECTORY="/home/austin/tftp_dir"
#    TFTP_ADDRESS="192.168.1.43:69"
#  sudo service udhcpd restart
#  sudo service tftpd-hpa restart

