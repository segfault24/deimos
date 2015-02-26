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

int strcmp(const char *s1, const char *s2)
{
	char *s1p, *s2p;

	s1p = (char *)s1;
	s2p = (char *)s2;
	while(*s1p != '\0' && *s2p != '\0' && *s1p == *s2p)
	{
		s1p++;
		s2p++;
	}
	if(*s1p == *s2p)
	{
		return 0;
	} else if(*s1p > *s2p) {
		return 1;
	} else {
		return -1;
	}
}
