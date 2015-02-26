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
