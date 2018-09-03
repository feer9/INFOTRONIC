#ifndef _STRING_H
#define _STRING_H

//#include <stddef.h>
#include "types.h"


size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char *index(const char *s, int c);
char *rindex(const char *s, int c);
void extract_substr(char const *s, char *sub, size_t from, size_t to);
void reverse(char *str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char *res, int afterpoint);

#endif
