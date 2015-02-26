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

#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <stddef.h>

/* copying and moving */
void *memcpy(void * __restrict s1, const void * __restrict s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
char *strcpy(char * __restrict s1, const char * __restrict s2);
char *strncpy(char * __restrict s1, const char * __restrict s2, size_t n);

/* concatenation */
char *strcat(char * __restrict s1, const char * __restrict s2);
char *strncat(char * __restrict s1, const char * __restrict s2, size_t n);

/* comparison */
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
//int strcoll(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
//size_t strxfrm(char * __restrict s1, const char * __restrict s2, size_t n);

/* searching */
//void *memchr(const void *s, int c, size_t n);
//char *strchr(const char *s, int c);
//size_t strcspn(const char *s1, const char *s2);
//char *strpbrk(const char *s1, const char *s2);
//char *strrchr(const char *s, int c);
//size_t strspn(const char *s1, const char *s2);
//char *strstr(const char *s1, const char *s2);
//char *strtok(char * __restrict s1, const char * __restrict s2);

/* additional operations */
void *memset(void *s, int c, size_t n);
//char *strerror(int errnum);
size_t strlen(const char *s);

#endif
