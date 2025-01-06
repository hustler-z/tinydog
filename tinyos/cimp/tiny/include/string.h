// @Hustler's Project

#ifndef _TINY_STRING_H_
#define _TINY_STRING_H_

#include <type.h>

char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t count);
char *strcat(char *dst, const char *src);
char *strncat(char *dst, const char *src, size_t count);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, size_t count);
size_t strnlen(const char *s, size_t count);
size_t strlen(const char *s);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strstr(const char *s1, const char *s2);
char *strnstr(const char *s1, const char *s2, size_t len);
char *strnchr(const char *s, size_t count, int c);
size_t strlcpy(char *dst, const char *src, size_t size);
size_t strlcat(char *dst, const char *src, size_t count);

void *memset(void *s, int c, size_t count);
void *memchr(const void *s, int c, size_t n);
void *memscan(void *addr, int c, size_t size);
int memcmp(const void *cs, const void *ct, size_t count);
void *memmove(void *dst, const void *src, size_t count);
void *memcpy(void *dst, const void *src, size_t count);

#endif /* _TINY_STRING_H_ */
