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
#include <kernel/error.h>

#include <kernel/disk.h>
#include <kernel/ext2.h>
#include <kernel/vfs.h>

void vfs_init()
{
}

void mount(char* volume, char* filesystem)
{
	volume++;//dummy
	// find which volume_t
	// check that it's not already mounted
	
	if(!strcmp(filesystem, "ext2"))
	{
		//ext2_open(v);
	}
	else if(!strcmp(filesystem, "fat32"))
	{
		//fat32_open(v);
	}
	else
	{
		kwarn("Specified filesystem not supported");
	}
}

void read_file(char* filename)
{
	filename++;//dummy
	// figure out which fs_node_t
}
