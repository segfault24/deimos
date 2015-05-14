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

#ifndef KERNEL_NET_H
#define KERNEL_NET_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/arp.h>

#define ETHERTYPE_IP4 0x0800
#define ETHERTYPE_ARP 0x0806

typedef struct _net_dev_t {
	struct _net_dev_t* next;
} net_dev_t;

typedef struct _pkt_buf_t {
	size_t len;
	uint8_t* data;
} pkt_buf_t;

typedef struct _ether_pkt_t {
	uint16_t dst_hi;
	uint16_t dst_mid;
	uint16_t dst_lo;
	uint16_t src_hi;
	uint16_t src_mid;
	uint16_t src_lo;
	uint16_t ethertype;
	uint8_t* payload;
} __attribute__((packed)) ether_pkt_t;

void net_init();

// net dev operations, return non-zero on failure
int register_net_dev(net_dev_t* dev);
int remove_net_dev(net_dev_t* dev);
void net_rx_pkt(net_dev_t* dev, pkt_buf_t* pkt);

// packet operations
pkt_buf_t* net_alloc_pkt_buf(size_t size);
void net_cpy_pkt(void* src, pkt_buf_t* dest, size_t size);

#endif
