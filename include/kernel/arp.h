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

#ifndef KERNEL_ARP_H
#define KERNEL_ARP_H

#include <stdint.h>
#include <kernel/ether.h>
#include <kernel/ip.h>

#define HTYPE_ETHERNET 1
#define PTYPE_IP4 ETHERTYPE_IP4
#define HLEN_ETHERNET 6
#define PLEN_IP4 4
#define OPER_REQUEST 1
#define OPER_REPLY 2

typedef struct _arp_pkt_t {
	uint16_t htype;					// hardware type
	uint16_t ptype;					// protocol type (see ethernet's ethertype)
	uint8_t hlen;					// hardware address length
	uint8_t plen;					// protocol address length
	uint16_t oper;					// operation
	// TODO: generalize this to arbitrary HTYPEs and PTYPEs
	uint8_t sha[HLEN_ETHERNET];		// sender hardware address
	uint8_t spa[PLEN_IP4];			// sender protocol address
	uint8_t tha[HLEN_ETHERNET];		// target hardware address
	uint8_t tpa[PLEN_IP4];			// target protocol address
} __attribute__((packed)) arp_pkt_t;

void arp_rx(arp_pkt_t* arp);
void arp_tx(arp_pkt_t* arp);
ether_addr_t arp_resolve(ip_addr_t ip);
void arp_flush();

void arp_print_info();

#endif
