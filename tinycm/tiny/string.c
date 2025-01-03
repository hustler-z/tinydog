// @Hustler's Project

#include <string.h>

char *strcpy(char *dst, const char *src)
{
    char *tmp = dst;

    while ((*dst++ = *src++) != '\0')
        /* nothing */;
    return tmp;
}

char *strncpy(char *dst, const char *src, size_t count)
{
    char *tmp = dst;

    while (count) {
        if ((*tmp = *src) != 0)
            src++;
        tmp++;
        count--;
    }
    return dst;
}

char *strcat(char *dst, const char *src)
{
    char *tmp = dst;

    while (*dst)
        dst++;
    while ((*dst++ = *src++) != '\0')
        ;
    return tmp;
}

char *strncat(char *dst, const char *src, size_t count)
{
    char *tmp = dst;

    if (count) {
        while (*dst)
            dst++;
        while ((*dst++ = *src++) != 0) {
            if (--count == 0) {
                *dst = '\0';
                break;
            }
        }
    }
    return tmp;
}

int strcmp(const char *cs, const char *ct)
{
    unsigned char c1, c2;

    while (1) {
        c1 = *cs++;
        c2 = *ct++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1)
            break;
    }
    return 0;
}

int strncmp(const char *cs, const char *ct, size_t count)
{
    unsigned char c1, c2;

    while (count) {
        c1 = *cs++;
        c2 = *ct++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1)
            break;
        count--;
    }
    return 0;
}

char *strchr(const char *s, int c)
{
    for (; *s != (char)c; ++s)
        if (*s == '\0')
            return NULL;
    return (char *)s;
}

char *strrchr(const char *s, int c)
{
    const char *last = NULL;
    do {
        if (*s == (char)c)
            last = s;
    } while (*s++);
    return (char *)last;
}

char *strnchr(const char *s, size_t count, int c)
{
    while (count--) {
        if (*s == (char)c)
            return (char *)s;
        if (*s++ == '\0')
            break;
    }
    return NULL;
}

size_t strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
        /* nothing */;
    return sc - s;
}

size_t strnlen(const char *s, size_t count)
{
    const char *sc;

    for (sc = s; count-- && *sc != '\0'; ++sc)
        /* nothing */;
    return sc - s;
}

void *memset(void *s, int c, size_t count)
{
    char *xs = s;

    while (count--)
        *xs++ = c;
    return s;
}

void *memcpy(void *dst, const void *src, size_t count)
{
    char *tmp = dst;
    const char *s = src;

    while (count--)
        *tmp++ = *s++;
    return dst;
}

void *memmove(void *dst, const void *src, size_t count)
{
    char *tmp;
    const char *s;

    if (dst <= src) {
        tmp = dst;
        s = src;
        while (count--)
            *tmp++ = *s++;
    } else {
        tmp = dst;
        tmp += count;
        s = src;
        s += count;
        while (count--)
            *--tmp = *--s;
    }
    return dst;
}

int memcmp(const void *cs, const void *ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}

void *memscan(void *addr, int c, size_t size)
{
    unsigned char *p = addr;

    while (size) {
        if (*p == c)
            return (void *)p;
        p++;
        size--;
    }
    return (void *)p;
}

char *strstr(const char *s1, const char *s2)
{
    size_t l1, l2;

    l2 = strlen(s2);
    if (!l2)
        return (char *)s1;
    l1 = strlen(s1);
    while (l1 >= l2) {
        l1--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}

char *strnstr(const char *s1, const char *s2, size_t len)
{
    size_t l2;

    l2 = strlen(s2);
    if (!l2)
        return (char *)s1;
    while (len >= l2) {
        len--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}

void *memchr(const void *s, int c, size_t n)
{
    const unsigned char *p = s;
    while (n-- != 0) {
            if ((unsigned char)c == *p++) {
            return (void *)(p - 1);
        }
    }
    return NULL;
}

size_t strlcat(char *dst, const char *src, size_t count)
{
    size_t dsize = strlen(dst);
    size_t len = strlen(src);
    size_t res = dsize + len;

    /* This would be a bug */
    if (dsize >= count)
        return 0;

    dst += dsize;
    count -= dsize;
    if (len >= count)
        len = count-1;
    memcpy(dst, src, len);
    dst[len] = 0;
    return res;
}

size_t strlcpy(char *dst, const char *src, size_t size)
{
    size_t ret = strlen(src);

    if (size) {
        size_t len = (ret >= size) ? size - 1 : ret;
        memcpy(dst, src, len);
        dst[len] = '\0';
    }
    return ret;
}
