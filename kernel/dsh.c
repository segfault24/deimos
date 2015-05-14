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
#include <kernel/tty.h>
#include <kernel/mm.h>
#include <kernel/kalloc.h>
#include <kernel/pci.h>
#include <kernel/disk.h>

#include <i386/isr.h>

extern int rtl_module_init();
extern int rtl_module_kill();
extern int ata_module_init();
extern int ata_module_kill();

void dsh_loop()
{
	char* buf;
	char c;
	int i;
	
	printf("DeimOS (c)2015 Built on %s at %s\n", __DATE__, __TIME__);
	
	buf = kmalloc(256);
	while(1)
	{
		i = 0;
		printf(">");
		while(i<255)
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
		{
			printf("clear | lspci | lsmem | lsheap | lsint | lsdisk\n");
			printf("rtlstart | rtlstop | atastart | atastop\n");
		}
		else if(!strcmp(buf, "clear")) tty_clear();
		else if(!strcmp(buf, "lspci")) pci_print_info();
		else if(!strcmp(buf, "lsmem")) memory_print_info();
		else if(!strcmp(buf, "lsheap")) heap_print_info();
		else if(!strcmp(buf, "lsint")) interrupts_print_info();
		else if(!strcmp(buf, "lsdisk")) disk_print_info();
		else if(!strcmp(buf, "rtlstart")) rtl_module_init();
		else if(!strcmp(buf, "rtlstop")) rtl_module_kill();
		else if(!strcmp(buf, "atastart")) ata_module_init();
		else if(!strcmp(buf, "atastop")) ata_module_kill();
		else if(!strcmp(buf, "")) putchar(' ');
		else printf("invalid command\n");
	}
	kfree(buf);
}
