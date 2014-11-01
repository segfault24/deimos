#include <stddef.h>
#include <stdint.h>
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
        term_buf[term_row*VGA_HEIGHT+term_col] = term_color | c;
        term_col++;
    } else {
        term_row++;
        term_col = 0;
    }
    if(term_col >= VGA_WIDTH)
    {
        term_col = 0;
        term_row++;
        if(term_row >= VGA_HEIGHT)
        {
            term_row = VGA_HEIGHT-1;
            term_scroll();
        }
    }
}

/* library implementations */

void term_setcolor(uint8_t bg, uint8_t fg)
{
    term_color = (fg | bg<<4) << 8;
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

void term_scroll()
{
    size_t i,j,tmp;

    for(i=1; i<VGA_HEIGHT; i++)
    {
        for(j=0; j<VGA_WIDTH; j++)
        {
            term_buf[(i-1)*VGA_HEIGHT+j] = term_buf[i*VGA_HEIGHT+j];
        }
    }
    tmp = (VGA_HEIGHT-1)*VGA_HEIGHT;
    for(j=0; j<VGA_WIDTH; j++)
    {
        term_buf[tmp+j] = ' ';
    }
}

void term_clear()
{
	size_t i;

	for(i=0; i<VGA_HEIGHT*VGA_WIDTH; i++)
		term_putchar_t(' ');
}

void term_init()
{
    term_buf = (uint16_t*)VGA_BASE_ADDR;
    term_setcolor(VGA_BLACK, VGA_GREEN);
	term_clear();
    term_row = 0;
    term_col = 0;
}
