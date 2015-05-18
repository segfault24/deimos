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

#ifndef KERNEL_ETHER_H
#define KERNEL_ETHER_H

#include <stdint.h>

#define ETHERTYPE_IP4 0x0800
#define ETHERTYPE_ARP 0x0806

typedef struct _ether_addr_t {
	uint8_t octet[6];
} __attribute__((packed)) ether_addr_t;

typedef struct _ether_pkt_t {
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t ethertype;
	uint8_t* payload;
} __attribute__((packed)) ether_pkt_t;

void ether_rx(ether_pkt_t* ether);
ether_addr_t ether_get_local();
ether_addr_t ether_get_broadcast();
int ether_cmp(ether_addr_t a, ether_addr_t b);

#endif
