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

#include <kernel/ctype.h>
#include <kernel/stdlib.h>

int atoi(const char* str)
{
	int i = 0;
	int sign = 1;
	
	while(*str && isspace(*str))
	{
		str++;
	}
	
	if(*str == '-')
	{
		sign = -1;
		str++;
	}
	else if(*str == '+')
	{
		str++;
	}
	
	while(*str && isdigit(*str))
	{
		i = i*10 + (*str - '0');
		str++;
	}
	
	return sign*i;
}
