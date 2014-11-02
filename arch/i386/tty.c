#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <i386/ioasm.h>
#include <kernel/tty.h>

/* internal variables and functions */

uint16_t* term_buf;
uint8_t term_color;

uint16_t term_row;
uint16_t term_col;

void term_updatecursor()
{
	uint16_t tmp;
	tmp = term_row*VGA_WIDTH+term_col;
	outb(VGA_INDEX_REG, 14);
	outb(VGA_INDEX_REG+1, (uint8_t)((tmp>>8)&0xFF));
	outb(VGA_INDEX_REG, 15);
	outb(VGA_INDEX_REG+1, (uint8_t)(tmp&0xFF));
}

// doesn't update cursor so that we don't waste
// time updating it while writing a string
void term_putchar_t(char c)
{
    if(c != '\n')
    {
        term_buf[term_row*VGA_WIDTH+term_col] = term_color<<8 | c;
        term_col++;
    }
    if(term_col >= VGA_WIDTH || c == '\n')
    {
        term_col = 0;
        term_row++;
        if(term_row >= VGA_HEIGHT)
        {
            term_scroll();
            term_row = VGA_HEIGHT-1;
        }
    }
}

/* library implementations */

void term_setcolor(uint8_t bg, uint8_t fg)
{
    term_color = fg | bg<<4;
}

void term_putchar(char c)
{
	term_putchar_t(c);
	term_updatecursor();
}

void term_puts(const char* str)
{
    size_t i;
    size_t len;

    len = strlen(str);
    for(i=0; i<len; i++)
        term_putchar_t(str[i]);
	term_updatecursor();
}

void term_puti(uint32_t i)
{
	size_t j;

	term_puts("0x");
	for(j=8; j>0; j--)
	{
		uint8_t hc = (i>>((j-1)*4)) & 0x0F;
		if(hc < 0x0A)
		{
			term_putchar((char)(hc+0x30)); // 0-9
		} else {
			term_putchar((char)(hc+0x37)); // A-F
		}
	}
}

void term_scroll()
{
    size_t r, c, tmp;

    for(r=1; r<VGA_HEIGHT; r++)
    {
        for(c=0; c<VGA_WIDTH; c++)
        {
            term_buf[(r-1)*VGA_WIDTH+c] = term_buf[r*VGA_WIDTH+c];
        }
    }
    tmp = (VGA_HEIGHT-1)*VGA_WIDTH;
    for(c=0; c<VGA_WIDTH; c++)
    {
        term_buf[tmp+c] = ' ';
    }
}

void term_clear()
{
	size_t i;

	term_row = 0;
	term_col = 0;
	// clear the buffer directly (faster than putchar)
	for(i=0; i<VGA_HEIGHT*VGA_WIDTH; i++)
		term_buf[i] = term_color<<8 | ' ';
	term_updatecursor();
}

void term_init()
{
    term_buf = (uint16_t*)VGA_BASE_ADDR;
    term_setcolor(VGA_BLACK, VGA_GREEN);
	term_clear();
}
