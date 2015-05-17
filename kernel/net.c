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

#include <stddef.h>
#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>
#include <kernel/ether.h>
#include <kernel/net.h>

static net_dev_t* net_dev_list = 0;

void net_init()
{
}

int register_net_dev(net_dev_t* dev)
{
	if(!dev)
		return 1;
	
	switch(dev->type)
	{
		case NETDEV_ETHERNET:
			break; // continue
		default:
			return 1; // bad device type or unsupported link protocol
	}
	
	dev->next = 0;
	if(!net_dev_list)
	{
		net_dev_list = dev;
		return 0;
	}
	
	net_dev_t* n = net_dev_list;
	while(n->next)
		n = n->next;
	n->next = dev;
	return 0;
}

int remove_net_dev(net_dev_t* dev)
{
	if(!dev || !net_dev_list)
		return 1;
	
	if(dev == net_dev_list)
	{
		net_dev_list = dev->next;
		dev->next = 0;
		return 0;
	}
	
	net_dev_t* n = net_dev_list;
	while(n->next)
	{
		if(n->next == dev)
		{
			n->next = n->next->next;
			dev->next = 0;
			return 0;
		}
		n = n->next;
	}
	return 1;
}

pkt_buf_t* net_alloc_pkt_buf(size_t size)
{
	// allocate the data buffer and struct at the same time
	pkt_buf_t* p = kmalloc(size + sizeof(pkt_buf_t));
	if(!p)
		return 0;
	p->data = (((void*)p) + sizeof(pkt_buf_t));
	return p;
}

void net_cpy_pkt(void* src, pkt_buf_t* dest, size_t size)
{
	memcpy(dest->data, src, size);
	dest->len = size;
}

void net_rx_pkt(net_dev_t* dev, pkt_buf_t* pkt)
{
	//printf("received packet from dev:%x len:%x\n", dev, pkt->len);
	
	// TODO: check for valid dev and pkt
	// TODO: this should really only queue the packet for another thread to process
	
	// send to proper protocol layer
	switch(dev->type)
	{
		case NETDEV_ETHERNET:
			ether_rx((ether_pkt_t*) pkt->data);
			break;
		default:
			break;
	}
	
	kfree(pkt);
}
