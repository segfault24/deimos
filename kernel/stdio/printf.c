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

#include <kernel/stdlib.h>
#include <kernel/stdio.h>

// TODO: incomplete
void printf(const char* format, ...)
{
	char **arg = (char**)&format;
	int c;
	char buf[20];
	
	arg++;
	
	while((c = *format++) != 0)
	{
		if (c != '%')
		putchar(c);
		else
		{
			char *p;
			c = *format++;
			switch(c)
			{
				case 'd':
				case 'u':
				case 'x':
					itoa(*((int *) arg++), buf, c);
					p = buf;
					goto string;
					break;
				
				case 's':
					p = *arg++;
					if (! p)
						p = "(null)";
					goto string;
					break;
				
				string:
					while (*p)
						putchar(*p++);
					break;
				
				default:
					putchar(*((int *) arg++));
					break;
			}
		}
	}
}
