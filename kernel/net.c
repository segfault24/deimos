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

#include <kernel/net.h>

static net_dev_t* net_dev_list = 0;

void net_init()
{
}

void net_loop()
{
}

int net_register_dev(net_dev_t* dev)
{
	net_dev_t* n;
	
	dev->next = 0;
	if(!net_dev_list)
	{
		net_dev_list = dev;
		return 1;
	}
	
	n = net_dev_list;
	while(n->next)
		n = n->next;
	n->next = dev;
	return 1;
}

int net_remove_dev(net_dev_t* dev)
{
	net_dev_t* n;
	
	if(!net_dev_list)
		return 1;
	
	if(dev == net_dev_list)
	{
		net_dev_list = dev->next;
		dev->next = 0;
		return 1;
	}
	
	n = net_dev_list;
	while(n->next)
	{
		if(n->next == dev)
		{
			n->next = n->next->next;
			dev->next = 0;
			return 1;
		}
		n = n->next;
	}
	return 0;
}

pkt_buf_t* net_alloc_pkt_buf(size_t size)
{
	size++; // dummy
	return 0;
}

void net_rx_pkt(net_dev_t* dev, pkt_buf_t* pkt)
{
	dev++; // dummy
	pkt++; // dummy
}
