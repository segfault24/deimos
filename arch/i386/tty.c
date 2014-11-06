#include <stddef.h>
#include <stdint.h>
#include <i386/ps2.h>
#include <i386/ioasm.h>
#include <kernel/tty.h>

/* internal variables and functions */

static uint16_t* tty_buf;
static uint8_t tty_color;

static uint16_t tty_row;
static uint16_t tty_col;

static void tty_updatecursor()
{
	uint16_t tmp;
	tmp = tty_row*VGA_WIDTH+tty_col;
	outb(VGA_INDEX_REG, 14);
	outb(VGA_INDEX_REG+1, (uint8_t)((tmp>>8)&0xFF));
	outb(VGA_INDEX_REG, 15);
	outb(VGA_INDEX_REG+1, (uint8_t)(tmp&0xFF));
}

static void tty_scroll()
{
    size_t r, c, tmp;

    for(r=1; r<VGA_HEIGHT; r++)
    {
        for(c=0; c<VGA_WIDTH; c++)
        {
            tty_buf[(r-1)*VGA_WIDTH+c] = tty_buf[r*VGA_WIDTH+c];
        }
    }
    tmp = (VGA_HEIGHT-1)*VGA_WIDTH;
    for(c=0; c<VGA_WIDTH; c++)
    {
        tty_buf[tmp+c] = ' ';
    }
}

// doesn't update cursor so that we don't waste
// time updating it while writing a string
static void tty_putchar_t(char c)
{
    if(c != '\n')
    {
        tty_buf[tty_row*VGA_WIDTH+tty_col] = tty_color<<8 | c;
        tty_col++;
    }
    if(tty_col >= VGA_WIDTH || c == '\n')
    {
        tty_col = 0;
        tty_row++;
        if(tty_row >= VGA_HEIGHT)
        {
            tty_scroll();
            tty_row = VGA_HEIGHT-1;
        }
    }
}

/* library implementations */

void tty_setcolor(uint8_t bg, uint8_t fg)
{
    tty_color = fg | bg<<4;
}

void tty_putchar(char c)
{
	tty_putchar_t(c);
	tty_updatecursor();
}

// TODO: mmmm, dat blocking IO
char tty_getchar()
{
	char c;
	do
	{
		c = kbd_getchar();
	} while(c=='\0');
	kbd_clearchar();
	return c;
}

void tty_puts(const char* str)
{
	size_t i = 0;
	while(str[i] != '\0')
	{
		tty_putchar_t(str[i]);
		i++;
	}
	tty_updatecursor();
}

void tty_puti(uint32_t i)
{
	size_t j;

	for(j=8; j>0; j--)
	{
		uint8_t hc = (i>>((j-1)*4)) & 0x0F;
		if(hc < 0x0A)
		{
			tty_putchar((char)(hc+0x30)); // 0-9
		} else {
			tty_putchar((char)(hc+0x37)); // A-F
		}
	}
}

void tty_clear()
{
	size_t i;

	tty_row = 0;
	tty_col = 0;
	// clear the buffer directly (faster than putchar)
	for(i=0; i<VGA_HEIGHT*VGA_WIDTH; i++)
		tty_buf[i] = tty_color<<8 | ' ';
	tty_updatecursor();
}

void tty_init()
{
    tty_buf = (uint16_t*)VGA_BASE_ADDR;
    tty_setcolor(VGA_BLACK, VGA_GREEN);
	tty_clear();
}
