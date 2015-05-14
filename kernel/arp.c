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
#include <kernel/string.h>
#include <kernel/endian.h>
#include <kernel/net.h>
#include <kernel/arp.h>

static void print_mac(uint16_t hi, uint16_t mid, uint16_t lo)
{
	printf("%x:%x:%x:%x:%x:%x",
		(uint8_t)(hi >> 8),
		(uint8_t)(hi >> 0),
		(uint8_t)(mid >> 8),
		(uint8_t)(mid >> 0),
		(uint8_t)(lo >> 8),
		(uint8_t)(lo >> 0)
	);
}

static void print_ip(uint32_t p)
{
	printf("%u.%u.%u.%u",
		(uint8_t)(p >> 0),
		(uint8_t)(p >> 8),
		(uint8_t)(p >> 16),
		(uint8_t)(p >> 24)
	);
}

void arp_rx(arp_pkt_t* arp)
{
	/*uint8_t* b = (uint8_t*)arp;
	size_t i;
	for(i=0; i<28; i++)
	{
		printf("%x ", *b);
		b++;
	}
	printf("\n");*/
	
	// validate arp packet
	if(
		ENDIANSWAP16(arp->htype) != HTYPE_ETHERNET ||
		ENDIANSWAP16(arp->ptype) != PTYPE_IP4 ||
		arp->hlen != HLEN_ETHERNET ||
		arp->plen != PLEN_IP4 ||
		(ENDIANSWAP16(arp->oper) != OPER_REQUEST &&
		ENDIANSWAP16(arp->oper) != OPER_REPLY))
	{
		printf("arp bad packet | htype:%x ptype:%x hlen:%x plen:%x oper:%x\n",
			ENDIANSWAP16(arp->htype),
			ENDIANSWAP16(arp->ptype),
			arp->hlen,
			arp->plen,
			ENDIANSWAP16(arp->oper));
		return;
	}
	
	printf("arp op:%x\n", arp->oper);
	
	printf("  src\t");
	print_mac(arp->sha_hi, arp->sha_mid, arp->sha_lo);
	printf("\t");
	print_ip(arp->spa);
	printf("\n");
	
	printf("  dst\t");
	print_mac(arp->tha_hi, arp->tha_mid, arp->tha_lo);
	printf("\t");
	print_ip(arp->tpa);
	printf("\n");
}
