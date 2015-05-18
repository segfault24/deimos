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
#include <kernel/ip.h>

static ip_addr_t our_ip;

void ip_rx(ip_pkt_t* ip)
{
	ip++; // dummy
	/*printf("ip pro=%u src=%u.%u.%u.%u dst=%u.%u.%u.%u\n",
		ip->protocol,
		ip->source.octet[0], ip->source.octet[1],
		ip->source.octet[2], ip->source.octet[3],
		ip->destination.octet[0], ip->destination.octet[1],
		ip->destination.octet[2], ip->destination.octet[3]
	);*/
}

ip_addr_t ip_get_local()
{
	our_ip.octet[0] = 12;
	our_ip.octet[1] = 34;
	our_ip.octet[2] = 56;
	our_ip.octet[3] = 78;
	return our_ip;
}

int ip_cmp(ip_addr_t a, ip_addr_t b)
{
	return
		a.octet[0] == b.octet[0] &&
		a.octet[1] == b.octet[1] &&
		a.octet[2] == b.octet[2] &&
		a.octet[3] == b.octet[3];
}
