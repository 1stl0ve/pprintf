#ifndef PPRINTF_H
#define PPRINTF_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

/* Define constants to use for formatting. */
#define ITALIC_C 'I'
#define ITALIC "\x1b[3m"
#define BOLD_C 'D'
#define BOLD "\x1b[1m"
#define UNDERLINE_C 'U'
#define UNDERLINE "\x1b[4m"
#define RED_C 'R'
#define RED "\x1b[31m"
#define GREEN_C 'G'
#define GREEN "\x1b[32m"
#define YELLOW_C 'Y'
#define YELLOW "\x1b[33m"
#define BLUE_C 'B'
#define BLUE "\x1b[34m"
#define MAGENTA_C 'M'
#define MAGENTA "\x1b[35m"
#define CYAN_C 'C'
#define CYAN "\x1b[36m"
#define WHITE_C 'W'
#define WHITE "\x1b[37m"
#define TOTAL_OPTIONS 10

/* Export functions */
int pprintf(const char *, ...);
char *pprintf_value(const char *, va_list); // Exported for test suite 
char *get_pprintf_value(const char *, ...);

#endif
