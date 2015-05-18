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

#include <kernel/stdio.h>
#include <kernel/endian.h>
#include <kernel/ip.h>
#include <kernel/arp.h>
#include <kernel/ether.h>

static ether_addr_t our_ether;

void ether_rx(ether_pkt_t* ether)
{
	// check ethertype
	switch(ENDIANSWAP16(ether->ethertype))
	{
		case ETHERTYPE_ARP:
			arp_rx((arp_pkt_t*) &(ether->payload));
			break;
		case ETHERTYPE_IP4:
			ip_rx((ip_pkt_t*) &(ether->payload));
			break;
		default:
			//printf("unsupported ethertype: 0x%x\n", ENDIANSWAP16(ether->ethertype));
			break;
	}
}

ether_addr_t ether_get_local()
{
	our_ether.octet[0] = 0x01;
	our_ether.octet[1] = 0x23;
	our_ether.octet[2] = 0x45;
	our_ether.octet[3] = 0x67;
	our_ether.octet[4] = 0x89;
	our_ether.octet[5] = 0xAB;
	return our_ether;
}

ether_addr_t ether_get_broadcast()
{
	ether_addr_t b;
	b.octet[0] = 0xFF;
	b.octet[1] = 0xFF;
	b.octet[2] = 0xFF;
	b.octet[3] = 0xFF;
	b.octet[4] = 0xFF;
	b.octet[5] = 0xFF;
	return b;
}

int ether_cmp(ether_addr_t a, ether_addr_t b)
{
	return
		a.octet[0] == b.octet[0] &&
		a.octet[1] == b.octet[1] &&
		a.octet[2] == b.octet[2] &&
		a.octet[3] == b.octet[3] &&
		a.octet[4] == b.octet[4] &&
		a.octet[5] == b.octet[5];
}
