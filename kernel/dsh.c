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

#include <stddef.h>
#include <stdint.h>

#include <kernel/stdio.h>
#include <kernel/string.h>
#include <kernel/kalloc.h>
#include <kernel/pci.h>

extern int module_init();
extern int module_kill();

void dsh_loop()
{
	char* buf;
	char c;
	int i;
	
	buf = kmalloc(256);
	while(1)
	{
		i = 0;
		printf(">");
		while(1)
		{
			c = getchar();
			putchar(c);
			if(c == '\n')
				break;
			buf[i] = c;
			i++;
		}
		buf[i] = '\0';
		
		if(!strcmp(buf, "help") || !strcmp(buf, "?"))
			printf("pcidump\nheapdump\nnetstart\nnetstop\n");
		else if(!strcmp(buf, "pcidump"))
			pci_dump();
		else if(!strcmp(buf, "heapdump"))
			kheap_print();
		else if(!strcmp(buf, "netstart"))
			module_init();
		else if(!strcmp(buf, "netstop"))
			module_kill();
		else
			printf("invalid command\n");
	}
	kfree(buf);
}
