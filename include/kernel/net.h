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

typedef struct _net_dev_t {
	struct _net_dev_t* next;
} net_dev_t;

typedef struct _pkt_buf_t {
	uint8_t* data;
	size_t len;
} pkt_buf_t;

void net_init();
void net_loop();

int net_register_dev(net_dev_t* dev);
int net_remove_dev(net_dev_t* dev);

pkt_buf_t* net_alloc_pkt_buf(size_t size);
void net_rx_pkt(net_dev_t* dev, pkt_buf_t* pkt);

#endif
