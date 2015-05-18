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

#ifndef KERNEL_IP_H
#define KERNEL_IP_H

#include <stdint.h>

#define PROTOCOL_ICMP 0x01
#define PROTOCOL_TCP  0x06
#define PROTOCOL_UDP  0x11

typedef struct _ip_addr_t {
	uint8_t octet[4];
} __attribute__((packed)) ip_addr_t;

typedef struct _ip_pkt_t {
	uint8_t version_ihl;
	uint8_t dscp_ecn;
	uint16_t length;
	uint16_t id;
	uint16_t flags_fragment;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	ip_addr_t source;
	ip_addr_t destination;
	uint8_t* payload;
} __attribute__((packed)) ip_pkt_t;

void ip_rx(ip_pkt_t* ip);
ip_addr_t ip_get_local();
int ip_cmp(ip_addr_t a, ip_addr_t b);

#endif
