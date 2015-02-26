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

char *strncpy(char * __restrict s1, const char * __restrict s2, size_t n)
{
	size_t i;
	char *s1p, *s2p;

	s1p = s1;
	s2p = (char *)s2;
	for(i=0; *s2p!='\0' && i<n; i++, s2p++, s1p++)
	{
		*s1p = *s2p;
	}
	for(; i<n; i++, s1p++)
	{
		*s1p = '\0';
	}
	return s1;
}
