#include <kernel/tty.h>

uint16_t* term_buf;
uint8_t term_color;

size_t term_row;
size_t term_col;

void term_setcolor(uint8_t bg, uint8_t fg)
{
    term_color = fg | bg<<4;
}

void term_putchar(char c)
{
    if(c != '\n')
    {
        term_buf[term_row*VGA_HEIGHT+term_col] = term_color<<8 | c;
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

void term_puts(const char* str)
{
    size_t i;
    size_t len;

    len = strlen(str);
    for(i=0; i<len; i++)
        term_putchar(str[i]);
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
		term_putchar(' ');
}

void term_init()
{
    term_buf = (uint16_t*)VGA_BASE_ADDR;
    term_setcolor(VGA_BLACK, VGA_GREEN);
	term_clear();
    term_row = 0;
    term_col = 0;
}
