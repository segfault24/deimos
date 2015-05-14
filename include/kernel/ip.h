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

#include <stddef.h>
#include <stdint.h>
#include <kernel/net.h>

typedef uint32_t ip_addr;

typedef struct _ip_pkt_t {
	uint32_t version		:4;
	uint32_t ihl			:4;
	uint32_t dscp			:6;
	uint32_t ecn			:2;
	uint32_t length			:16;
	uint32_t id				:16;
	uint32_t flags			:3;
	uint32_t fragment		:13;
	uint32_t ttl			:8;
	uint32_t protocol		:8;
	uint32_t checksum		:16;
	ip_addr source			:32;
	ip_addr destination		:32;
	uint8_t* payload;
} __attribute__((packed)) ip_pkt_t;

void ip_rx(ip_pkt_t* ip);

#endif
