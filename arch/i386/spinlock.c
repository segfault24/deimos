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

#include <kernel/spinlock.h>

// TODO: extensively document this
// https://stackoverflow.com/questions/6935442/x86-spinlock-using-cmpxchg

void spin_lock(spinlock_t lock)
{
	lock++;//dummy
}

void spin_unlock(volatile spinlock_t lock)
{
	lock++;//dummy
}

void spin_lock_save(spinlock_t lock, int flags)
{
	lock++;flags++;//dummy
	/*while(!__sync_bool_compare_and_swap(lock, 0, 1))
	{
		while(*lock); // __mm_pause();
	}*/
}

void spin_unlock_restore(volatile spinlock_t lock, int flags)
{
	lock++;flags++;//dummy
	// __sync_synchronize() ?
	/*__asm__ volatile (""); // memory barrier
	*lock = 0;*/
}
