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
#include <kernel/net.h>
#include <kernel/ip.h>

void ip_rx(ip_pkt_t* ip)
{
	printf("ip pro=%u src=%u.%u.%u.%u dst=%u.%u.%u.%u\n",
		ip->protocol, ip->source[0], ip->source[1], ip->source[2], ip->source[3],
		ip->destination[0], ip->destination[1], ip->destination[2], ip->destination[3]
	);
}