#ifndef STDIO_H
#define STDIO_H

//size_t
//FILE
//fpos_t
//NULL
//_IOFBF
//_IOLBF
//_IONBF
//BUFSIZ
//EOF
//FOPEN_MAX
//FILENAME_MAX
//L_tmpnam
//SEEK_CUR
//SEEK_END
//SEEK_SET
//TMP_MAX
//stderr
//stdin
//stdout

/* file operations */
//int remove(const char *filename);
//int rename(const char *old, const char *new);
//FILE *tmpfile(void);
//char *tmpnam(char *s);

/* file access */
//int fclose(FILE *stream);
//int fflush(FILE *stream);
//FILE *fopen(const char * restrict filename, const char * restrict mode);
//FILE *freopen(const char * restrict filename, const char * restrict mode, FILE * restrict stream);
//void setbuf(FILE * restrict stream, char * restrict buf);
//int setvbuf(FILE * restrict stream, char * restrict buf, int mode, size_t size);

/* formatted input & output */
//int fprintf(FILE * restrict stream, const char * restrict format, ...);
//int fscanf(FILE * restrict stream, const char * restrict format, ...);
//int printf(const char * restrict format, ...);
//int scanf(const char * restrict format, ...);
//int snprintf(char * restrict s, size_t n, const char * restrict format, ...);
//int sprintf(char * restrict s, const char * restrict format, ...);
//int sscanf(const char * restrict s, const char * restrict format, ...);
//int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg);
//int vfscanf(FILE * restrict stream, const char * restrict format, va_list arg);
//int vprintf(const char * restrict format, va_list arg);
//int vscanf(const char * restrict format, va_list arg);
//int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg);
//int vsprintf(char * restrict s, const char * restrict format, va_list arg);
//int vsscanf(const char * restrict s, const char * restrict format, va_list arg);

/* character input & output */
//int fgetc(FILE *stream);
//char *fgets(char * restrict s, int n, FILE * restrict stream);
//int fputc(int c, FILE *stream);
//int fputs(const char * restrict s, FILE * restrict stream);
//int getc(FILE *stream);
//int getchar(void);
//int putc(int c, FILE *stream);
//int putchar(int c);
//int puts(const char *s);
//int ungetc(int c, FILE *stream);

/* direct input & output */
//size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);
//size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);

/* file positioning */
//int fgetpos(FILE * restrict stream, fpos_t * restrict pos);
//int fseek(FILE *stream, long int offset, int whence);
//int fsetpos(FILE *stream, const fpos_t *pos);
//long int ftell(FILE *stream);
//void rewind(FILE *stream);

/* error handling */
//void clearerr(FILE *stream);
//int feof(FILE *stream);
//int ferror(FILE *stream);
//void perror(const char *s);

#endif

