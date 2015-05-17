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
