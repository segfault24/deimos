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

#include <kernel/string.h>

void *memcpy(void * __restrict s1, const void * __restrict s2, size_t n)
{
	size_t i;
	unsigned char *s1p, *s2p;

	s1p = (unsigned char *)s1;
	s2p = (unsigned char *)s2;
	for(i=0; i<n; i++)
		s1p[i] = s2p[i];
	return s1;
}
