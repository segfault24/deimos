/*
 *  Copyright (C) 2015  Austin Selick-Bottos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// TODO:
//    transmit capabilities
//    handle & track errors
//    is parsing PCI BARs our job, or the PCI subsystem's?
//    also, we didnt really check the BARs.. we just assumed BAR0 is I/O 

#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>
#include <kernel/error.h>
#include <i386/ioasm.h>
#include <i386/isr.h>
#include <kernel/pci.h>
#include <kernel/net.h>

#define RBSTART		0x30	// 4 bytes
#define CR			0x37	// 1
#define IMR			0x3C	// 2
#define ISR			0x3E	// 2
#define TCR			0x40	// 4
#define RCR			0x44	// 4
#define CONFIG0		0x51	// 1
#define CONFIG1		0x52	// 1
#define TSAD		0x60	// 2

static pci_dev_t* rtl_pci_dev;
static net_dev_t* rtl_net_dev;

static uint32_t iobase;

static void rtl_isr_handler()
{
	// clear RTL's interrupt lines
	outw(iobase + ISR, 0x0001);
}

int module_init()
{
	// start by searching for the device
	rtl_pci_dev = pci_search(0x10EC, 0x8139);
	if(!rtl_pci_dev)
		kpanic("rtl8139: could not find RTL8139 device");
	
	// enable bus mastering
	uint16_t c = pci_readw(rtl_pci_dev, CFG_W_COMMAND);
	pci_writew(rtl_pci_dev, CFG_W_COMMAND, c | 0x04);
	
	// grab the iobase
	iobase = pci_readl(rtl_pci_dev, CFG0_L_BAR0) & 0xFFFFFFFC;
	
	// turn it on & software reset
	outb(iobase + CONFIG1, 0x00);
	outb(iobase + CR, 0x10);
	while((inb(iobase + CR) & 0x10) != 0); // TODO: timeout?
	
	// setup our net_dev_t
	rtl_net_dev = kmalloc(sizeof(net_dev_t));
	if(!rtl_net_dev)
		kpanic("rtl8139: could not allocate memory for net dev");
	memset(rtl_net_dev, 0, sizeof(net_dev_t));
	
	// receive buffers setup
	
	
	// transmit buffers setup
	
	
	// configure receiver
	outl(iobase + RCR, 0x0000008F); // accept all packets, dont wrap rx buffer
	
	// register the net_dev_t
	if(register_net_dev(rtl_net_dev))
		kpanic("rtl8139: could not register net dev");
	
	// enable rx and tx
	outb(iobase + CR, 0x0C);
	
	// iterrupt setup
	if(request_irq(rtl_pci_dev->intline, (unsigned int)&rtl_pci_dev, &rtl_isr_handler))
		kpanic("rtl8139: could not register irq");
	outw(iobase + IMR, 0x0005); // enables TOK and ROK irqs on RTL
	
	return 0;
}

int module_kill()
{
	// clear the isr handle and the device's interrupts
	outw(iobase + IMR, 0x0000);
	release_irq(rtl_pci_dev->intline, (unsigned int)&rtl_pci_dev);
	
	// disable rx and tx
	outb(iobase + CR, 0x00);
	
	// unregister and free the net_dev_t
	if(remove_net_dev(rtl_net_dev))
		kpanic("rtl8139: could not remove net dev");
	if(rtl_net_dev)
		kfree(rtl_net_dev);
	
	// free the rx and tx buffers
	
	// put the device to sleep
	
	
	return 0;
}
