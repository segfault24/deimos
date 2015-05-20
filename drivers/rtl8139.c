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

#include <stddef.h>
#include <stdint.h>
#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>
#include <kernel/error.h>
#include <i386/ioasm.h>
#include <i386/isr.h>
#include <kernel/pci.h>
#include <kernel/net.h>

// I/O space register offsets
#define ID0			0x00	// 1		MAC address
#define ID1			0x01	// 1
#define ID2			0x02	// 1
#define ID3			0x03	// 1
#define ID4			0x04	// 1
#define ID5			0x05	// 1

#define RBSTART		0x30	// 4		
#define CR			0x37	// 1		command
#define CAPR		0x38	// 2		current address of packet read
#define IMR			0x3C	// 2		interrupt mask
#define ISR			0x3E	// 2		interrupt status
#define TCR			0x40	// 4		transmitter config
#define RCR			0x44	// 4		recevier config
#define CONFIG0		0x51	// 1
#define CONFIG1		0x52	// 1
#define TSAD		0x60	// 2

// command register bits
#define BUFE		0x01	// RX buffer empty
#define TE			0x04	// TX enable
#define RE			0x08	// RX enable
#define RST			0x10	// reset

// interrupt status bits
#define ROK			0x0001	// RX ok
#define RER			0x0002	// RX error
#define TOK			0x0004	// TX ok
#define TER			0x0008	// TX error

// receiver configuration bits
#define AAP			0x00000001	// accept all
#define APM			0x00000002	// accept physical match
#define AM			0x00000004	// accept multicast
#define AB			0x00000008	// accept broadcast
#define WRAP		0x00000080	// rx buffer wrap (0=wrap, 1=nowrap)
#define MXDMA64		0x00000200	// maximum DMA burst sizes
#define MXDMA128	0x00000300	//
#define MXDMA256	0x00000400	//
#define RBLEN8K		0x00000000	// receive buffer length
#define RBLEN16K	0x00000800	//
#define RBLEN32K	0x00001000	//
#define RBLEN64K	0x00001800	//

// our settings
#define RX_BUF_SIZE	32*1024
#define RX_BUF_PAD	16+1500
#define RX_CONFIG	RBLEN32K|MXDMA256|WRAP|AAP|APM|AM|AB

static pci_dev_t* rtl_pci_dev = 0;
static net_dev_t* rtl_net_dev = 0;

static int started = 0;
static unsigned int iobase;
static uint8_t mac[6];

static uint8_t* rx_buf = 0;
static unsigned int rx_buf_phys = 0;
static unsigned short rx_ptr = 0;

static void rtl_rx_reset()
{
	uint8_t tmp = inb(iobase + CR);
	outb(iobase + CR, tmp & ~RE);
	outb(iobase + CR, tmp);
	outl(iobase + RCR, RX_CONFIG);
}

static void rtl_isr_handler()
{
	// grab the interrupt bits
	uint16_t int_flags = inw(iobase + ISR);
	// clear RTL's interrupt lines
	outw(iobase + ISR, 0xFFFF);
	
	if(int_flags & ROK)
	{
		// while theres a packet in the buffer
		while((inb(iobase + CR) & BUFE) == 0)
		{
			// wrap the read pointer
			if(rx_ptr > RX_BUF_SIZE)
				rx_ptr = rx_ptr % RX_BUF_SIZE;
			
			// get and calculate the packet size
			unsigned int rx_header = *(unsigned int*)(rx_buf + rx_ptr);
			unsigned short rx_status = (unsigned short)rx_header;
			unsigned short rx_size = rx_header >> 16;
			unsigned short pkt_size = rx_size - 4;
			//putchar('.');
			//printf("rx_ptr:%x status:%x rxsz:%x ", rx_ptr, rx_status, rx_size);
			
			if(rx_status == 0) // TODO: check status first
			{
			}
			else
			{
				// allocate a packet buffer
				pkt_buf_t* pkt = net_alloc_pkt_buf(pkt_size);
				if(pkt)
				{
					// copy in the packet, send it up
					net_cpy_pkt(rx_buf + rx_ptr + 4, pkt, pkt_size);
					net_rx_pkt(rtl_net_dev, pkt);
				}
				else
				{
					// the alloc failed, drop the packet
					kwarn("rtl8139: could not allocate memory, dropped a packet");
				}
			}
			
			// update the read pointer
			rx_ptr = (rx_ptr + rx_size + 4 + 3) & ~0x3;
			//printf("new_ptr:%x\n", rx_ptr);
			
			// tell the RTL about it
			outb(iobase + CAPR, rx_ptr - 16);
		}
	}
	if(int_flags & ~ROK)
	{
		//putchar('e');
		//printf("err:%xh", int_flags);
		rtl_rx_reset();
	}
}

static void rtl_hardware_init()
{
	// grab the iobase from PCI config space
	iobase = pci_readl(rtl_pci_dev, CFG0_L_BAR0) & 0xFFFFFFFC;
	
	outb(iobase + CONFIG1, 0x00);			// turn it on
	outb(iobase + CR, RST);					// software reset
	while((inb(iobase + CR) & RST) != 0);	// wait for ready TODO: timeout?
	outb(iobase + CR, RE);					// enable rx (before transfer thresholds)
	outl(iobase + RCR, RX_CONFIG);			// receiver configuation
	outl(iobase + RBSTART, rx_buf_phys);	// set the rx buffer
	outw(iobase + IMR, 0xFFFF);				// enables irqs
	
	// grab the cards MAC address
	mac[0] = inb(iobase + ID0);
	mac[1] = inb(iobase + ID1);
	mac[2] = inb(iobase + ID2);
	mac[3] = inb(iobase + ID3);
	mac[4] = inb(iobase + ID4);
	mac[5] = inb(iobase + ID5);
}

int rtl_module_init()
{
	// make sure we're not running already
	if(started)
	{
		kwarn("rtl8139: already started");
		return 1; // no cleaning
	}
	
	// start by searching for the device
	rtl_pci_dev = pci_search(0x10EC, 0x8139);
	if(!rtl_pci_dev)
	{
		kwarn("rtl8139: could not find RTL8139 device");
		return 1;
	}
	
	// enable bus mastering
	uint16_t c = pci_readw(rtl_pci_dev, CFG_W_COMMAND);
	pci_writew(rtl_pci_dev, CFG_W_COMMAND, c | 0x04);
	
	// setup our net_dev_t
	rtl_net_dev = kmalloc(sizeof(net_dev_t));
	if(!rtl_net_dev)
	{
		kwarn("rtl8139: could not allocate memory for net dev");
		goto cleanandfail;
	}
	memset(rtl_net_dev, 0, sizeof(net_dev_t));
	rtl_net_dev->type = NETDEV_ETHERNET;
	
	// receive buffers setup
	// TODO: URGENT kmalloc_ap is not guaranteed to be physically contiguous
	rx_buf = kmalloc_ap(RX_BUF_SIZE + RX_BUF_PAD, (phys_addr*)&rx_buf_phys);
	if(!rx_buf)
	{
		kwarn("rtl8139: could not allocate memory for rx buffer");
		goto cleanandfail;
	}
	rx_ptr = 0;
	
	// transmit buffers setup
	// TODO: URGENT kmalloc_ap is not guaranteed to be physicall contiguous
	
	// register the net_dev_t
	if(register_net_dev(rtl_net_dev))
	{
		kwarn("rtl8139: could not register net dev");
		goto cleanandfail;
	}
	
	// iterrupt setup
	if(request_irq(rtl_pci_dev->intline, (unsigned int)&rtl_pci_dev, &rtl_isr_handler))
	{
		kwarn("rtl8139: could not register irq");
		goto cleanandfail;
	}
	
	rtl_hardware_init();
	
	started = 1;
	return 0;
	
	cleanandfail:
		kfree(rtl_net_dev);rtl_net_dev = 0;
		kfree(rx_buf);rx_buf = 0;
		//kfree(tx_buf);tx_buf = 0;
		return 1;
}

int rtl_module_kill()
{
	// make sure we're actually running
	if(!started)
	{
		kwarn("rtl8139: can't kill something that's not running");
		return 1;
	}
	
	// disable rx and tx
	outb(iobase + CR, 0x00);
	
	// clear the isr handle and the device's interrupts
	outw(iobase + IMR, 0x0000);
	release_irq(rtl_pci_dev->intline, (unsigned int)&rtl_pci_dev);
	
	// unregister and free the net_dev_t
	if(remove_net_dev(rtl_net_dev))
		kwarn("rtl8139: could not remove net dev");
	kfree(rtl_net_dev);
	
	// free the rx and tx buffers
	kfree(rx_buf);
	
	// TODO: put the device to sleep
	
	started = 0;
	return 0;
}
