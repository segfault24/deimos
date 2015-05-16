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
#include <kernel/ip.h>

void arp_rx(arp_pkt_t* arp)
{
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
	
	printf("arp op=%u src=%x:%x:%x:%x:%x:%x/%u.%u.%u.%u dst=%x:%x:%x:%x:%x:%x/%u.%u.%u.%u\n",
		ENDIANSWAP16(arp->oper),
		arp->sha[0], arp->sha[1], arp->sha[2], arp->sha[3], arp->sha[4], arp->sha[5],
		arp->spa[0], arp->spa[1], arp->spa[2], arp->spa[3],
		arp->tha[0], arp->tha[1], arp->tha[2], arp->tha[3], arp->tha[4], arp->tha[5],
		arp->tpa[0], arp->tpa[1], arp->tpa[2], arp->tpa[3]
	);
}
