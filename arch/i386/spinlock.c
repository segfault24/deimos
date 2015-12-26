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

void spin_lock(spinlock_t* lock)
{
	while(!__sync_bool_compare_and_swap(&lock->lockval, 0, 1))
	{
		while((volatile int)lock->lockval);
	}
}

void spin_unlock(spinlock_t* lock)
{
	__sync_synchronize(); // full memory barrier
	lock->lockval = 0;
}

void spin_lock_save(spinlock_t* lock, int flags)
{
	lock++;flags++;//dummy
}

void spin_unlock_restore(spinlock_t* lock, int flags)
{
	lock++;flags++;//dummy
}
