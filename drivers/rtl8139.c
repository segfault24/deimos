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

#include <kernel/kalloc.h>
#include <kernel/stdio.h>
#include <kernel/pci.h>
#include <kernel/net.h>

static pci_dev_t* rtl_pci_dev;
//static net_dev_t* rtl_net_dev;

int module_init()
{
	rtl_pci_dev = pci_search(0x10EC, 0x8139);
	if(!rtl_pci_dev)
		return 0;
	printf("found RTL8139 device\n");
	return 0;
}

int module_kill()
{
	return 0;
}
