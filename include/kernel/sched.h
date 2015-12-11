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

#ifndef KERNEL_SCHED_H
#define KERNEL_SCHED_H

#include <kernel/task.h>

#define KERNEL_SCHED_HZ 1000

void sched_init();
void sched_print_info();

void do_scheduling();

pid_t create_kernel_task(void (*func)(void));
void kill(pid_t pid);

void sched_yield();
void sched_kill();

unsigned int sleep(unsigned int seconds);
int usleep(unsigned int millis);

//int kfork();

#endif
