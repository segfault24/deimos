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
#include <kernel/error.h>
#include <kernel/endian.h>
#include <kernel/ether.h>
#include <kernel/ip.h>
#include <kernel/arp.h>

typedef struct _arp_cache_entry_t {
	ether_addr_t ether;
	ip_addr_t ip;
	struct _arp_cache_entry_t* next;
} arp_cache_entry_t;

static arp_cache_entry_t* arp_cache = 0;

static arp_cache_entry_t* arp_cache_search(ip_addr_t ip)
{
	printf("arp cache search\n");
	arp_cache_entry_t* e = arp_cache;
	while(e)
	{
		if(ip_cmp(e->ip, ip)) return e;
		e = e->next;
	}
	return 0;
}

void arp_rx(arp_pkt_t* arp)
{
	// TODO: ARP probes?
	// TODO: need threading & interrupt protection
	// TODO: how long do we keep entries? need timer functions for this
	
	// validate arp packet, return if it doesnt match what we support
	if(ENDIANSWAP16(arp->htype) != HTYPE_ETHERNET ||
		ENDIANSWAP16(arp->ptype) != PTYPE_IP4 ||
		arp->hlen != HLEN_ETHERNET ||
		arp->plen != PLEN_IP4 ||
		(ENDIANSWAP16(arp->oper) != OPER_REQUEST &&
		ENDIANSWAP16(arp->oper) != OPER_REPLY)) return;
	
	printf("arp rx %u src=%x:%x:%x:%x:%x:%x/%u.%u.%u.%u dst=%x:%x:%x:%x:%x:%x/%u.%u.%u.%u\n",
		ENDIANSWAP16(arp->oper),
		arp->sha.octet[0], arp->sha.octet[1], arp->sha.octet[2],
		arp->sha.octet[3], arp->sha.octet[4], arp->sha.octet[5],
		arp->spa.octet[0], arp->spa.octet[1], arp->spa.octet[2], arp->spa.octet[3],
		arp->tha.octet[0], arp->tha.octet[1], arp->tha.octet[2],
		arp->tha.octet[3], arp->tha.octet[4], arp->tha.octet[5],
		arp->tpa.octet[0], arp->tpa.octet[1], arp->tpa.octet[2], arp->tpa.octet[3]
	);
	
	// we take note of all packets regardless of whether they were for us
	arp_cache_entry_t* e = arp_cache_search(arp->spa);
	if(e)
	{
		printf("arp cache update\n");
		// update existing entry
		e->ether = arp->sha;
	}
	else
	{
		printf("arp cache new\n");
		////////////// THIS SECTION FUCKS UP THE HEAP SOMEHOW ////////////////
		// create new entry
		// e = kmalloc(sizeof(arp_cache_entry_t));
		// if(!e)
		// {
			// no memory, dont add entry
			// kwarn("arp: could not allocate memory for ARP cache entry");
			// return;
		// }
		// e->ip = arp->spa;
		// e->ether = arp->sha;
		// e->next = arp_cache;
		// arp_cache = e; // add it to the head (because that's easiest)
		//////////////////////////////////////////////////////////////////////
	}
	
	// if we're the target protocol address and it's a request, we need to send a reply
	if(ip_cmp(arp->tpa, ip_get_local()) && ENDIANSWAP16(arp->oper) == OPER_REQUEST)
	{
		printf("arp sending reply\n");
		// send a reply to the request
		// arp_pkt_t reply;
		// reply.htype = ENDIANSWAP16(HTYPE_ETHERNET);
		// reply.ptype = ENDIANSWAP16(PTYPE_IP4);
		// reply.hlen = HLEN_ETHERNET;
		// reply.plen = PLEN_IP4;
		// reply.oper = ENDIANSWAP16(OPER_REPLY);
		// reply.sha = ether_get_local();
		// reply.spa = ip_get_local();
		// reply.tha = arp->sha;
		// reply.tpa = arp->spa;
		// arp_tx(&reply);
	}
}

void arp_tx(arp_pkt_t* arp)
{
	// TODO: transmit
	
	printf("arp tx\n");
	arp++; // dummy
}

int arp_resolve(ip_addr_t ip, ether_addr_t* ether)
{
	printf("arp resolve\n");
	// search our cache
	arp_cache_entry_t* e = arp_cache_search(ip);
	if(e != 0)
	{
		// hit! set the caller's variable and return success
		*(ether) = e->ether;
		return 0;
	}
	
	printf("arp sending request\n");
	// didn't find an entry, so craft and send an ARP request,
	// and tell the caller we failed
	// arp_pkt_t arp;
	// arp.htype = ENDIANSWAP16(HTYPE_ETHERNET);
	// arp.ptype = ENDIANSWAP16(PTYPE_IP4);
	// arp.hlen = HLEN_ETHERNET;
	// arp.plen = PLEN_IP4;
	// arp.oper = ENDIANSWAP16(OPER_REQUEST);
	// arp.sha = ether_get_local(); // our MAC
	// arp.spa = ip_get_local(); // our IP
	// arp.tha = ether_get_broadcast(); // broadcast MAC
	// arp.tpa = ip; // target IP
	// arp_tx(&arp);
	return 1;
}

void arp_flush()
{
	printf("arp flush\n");
	arp_cache_entry_t* e = arp_cache;
	while(e)
	{
		arp_cache_entry_t* tmp = e;
		e = e->next;
		arp_cache = e;
		kfree(tmp);
	}
}

void arp_print_info()
{
	printf("arp table\n");
	arp_cache_entry_t* e = arp_cache;
	while(e)
	{
		printf("  %x  %x:%x:%x:%x:%x:%x  %u.%u.%u.%u  %x\n",
			e,
			e->ether.octet[0], e->ether.octet[1], e->ether.octet[2],
			e->ether.octet[3], e->ether.octet[4], e->ether.octet[5],
			e->ip.octet[0], e->ip.octet[1], e->ip.octet[2], e->ip.octet[3],
			e->next
		);
		e = e->next;
	}
}
