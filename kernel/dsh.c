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
#include <kernel/sched.h>
#include <kernel/pci.h>
#include <kernel/disk.h>
#include <kernel/arp.h>
#include <kernel/power.h>
#include <kernel/error.h>

#include <i386/isr.h>
#include <i386/pit.h>

extern int rtl_module_init();
extern int rtl_module_kill();
extern int ata_module_init();
extern int ata_module_kill();

static void task_test_loop()
{
	unsigned int i, j;
	for(i=0; i<20000; i++)
		for(j=0; j<20000; j++)
			__asm__ volatile ("");
}

static void task_test()
{
	int i;
	for(i=0; i<10; i++)
		create_kernel_task(task_test_loop);
}

static void heap_test()
{
	int i, j;
	void* ret[1000];
	for(i=0; i<1000; i++)
	{
		ret[i] = kmalloc_a(8192);
		printf("%x ", ret[i]);
		if(!ret[i])
		{
			printf("\nno more memory (%u allocs)\n", i);
			for(j=0; j<i; j++)
			{
				printf("%x ", ret[j]);
				kfree(ret[j]);
			}
			return;
		}
	}
	printf("success\n");
}

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
			c = putchar(getchar());
			if(c == '\n')
				break;
			else if(c == '*') // reboot shortcut for testing
				reboot();
			buf[i] = c;
			i++;
		}
		buf[i] = '\0';
		
		if(!strcmp(buf, "help") || !strcmp(buf, "?"))
		{
			printf("clear | reboot | pci | mem | heap | int | disk | sched | beep\n");
			printf("rtl <start|stop> | ata <start|stop> | arp <flush>\n");
		}
		else if(!strcmp(buf, "clear")) tty_clear();
		else if(!strcmp(buf, "reboot")) reboot();
		else if(!strcmp(buf, "pci")) pci_print_info();
		else if(!strcmp(buf, "mem")) memory_print_info();
		else if(!strcmp(buf, "heap")) heap_print_info();
		else if(!strcmp(buf, "int")) interrupts_print_info();
		else if(!strcmp(buf, "disk")) disk_print_info();
		else if(!strcmp(buf, "sched")) sched_print_info();
		else if(!strcmp(buf, "beep")) beep(2000, 1000);
		
		else if(!strcmp(buf, "rtl start")) rtl_module_init();
		else if(!strcmp(buf, "rtl stop")) rtl_module_kill();
		else if(!strcmp(buf, "ata start")) ata_module_init();
		else if(!strcmp(buf, "ata stop")) ata_module_kill();
		
		//else if(!strcmp(buf, "net")) net_print_info();
		//else if(!strcmp(buf, "ip")) ip_print_info();
		else if(!strcmp(buf, "arp")) arp_print_info();
		else if(!strcmp(buf, "arp flush")) arp_flush();
		
		// test functions
		else if(!strcmp(buf, "tasktest")) task_test();
		else if(!strcmp(buf, "heaptest")) heap_test();
		
		else if(!strcmp(buf, ""));
		else printf("invalid command \"%s\"\n", buf);
	}
	kfree(buf);
}
