// @Hustler's Project

// ####################################################
#include <util.h>
#include <type.h>
#include <compiler.h>
#include <string.h>

#define va_start(v,l)      __builtin_va_start((v),l)
#define va_end             __builtin_va_end
#define va_arg             __builtin_va_arg
typedef __builtin_va_list  va_list;

static int skip_atoi(const char **s)
{
    int i=0;

    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';

    return i;
}

#define ZEROPAD 1               /* pad with zero */
#define SIGN    2               /* unsigned/signed long */
#define PLUS    4               /* show plus */
#define SPACE   8               /* space if plus */
#define LEFT    16              /* left justified */
#define SPECIAL 32              /* 0x */
#define LARGE   64              /* use 'ABCDEF' instead of 'abcdef' */

static char *number(
    char *buf, const char *end, unsigned long long num,
    int base, int size, int precision, int type)
{
    char c,sign,tmp[66];
    const char *digits;
    static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    static const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;

    ASSERT(base >= 2 && base <= 36);

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;

    if (type & SIGN) {
        if ((signed long long) num < 0) {
            sign = '-';
            num = - (signed long long) num;
            size--;
        } else if (type & PLUS) {
            sign = '+';
            size--;
        } else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }

    if (type & SPECIAL) {
        if (num == 0)
            type &= ~SPECIAL;
        else if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
        else
            type &= ~SPECIAL;
    }

    i = 0;
    if (num == 0)
        tmp[i++]='0';
    else while (num != 0)
        tmp[i++] = digits[do_div(num, base)];
    if (i > precision)
        precision = i;

    size -= precision;

    if (!(type&(ZEROPAD + LEFT))) {
        while (size-- > 0) {
            if (buf < end)
                *buf = ' ';
            ++buf;
        }
    }

    if (sign) {
        if (buf < end)
            *buf = sign;
        ++buf;
    }

    if (type & SPECIAL) {
        if (buf < end)
            *buf = '0';
        ++buf;
        if (base == 16) {
            if (buf < end)
                *buf = digits[33];
            ++buf;
        }
    }

    if (!(type & LEFT)) {
        while (size-- > 0) {
            if (buf < end)
                *buf = c;
            ++buf;
        }
    }

    while (i < precision--) {
        if (buf < end)
            *buf = '0';
        ++buf;
    }

    while (i-- > 0) {
        if (buf < end)
            *buf = tmp[i];
        ++buf;
    }

    while (size-- > 0) {
        if (buf < end)
            *buf = ' ';
        ++buf;
    }

    return buf;
}

static char *string(char *str, const char *end, const char *s,
                    int field_width, int precision, int flags)
{
    int i, len = (precision < 0) ? strlen(s) : strnlen(s, precision);

    if (!(flags & LEFT)) {
        while (len < field_width--) {
            if (str < end)
                *str = ' ';
            ++str;
        }
    }

    for (i = 0; i < len; ++i) {
        if (str < end)
            *str = *s;
        ++str; ++s;
    }

    while (len < field_width--) {
        if (str < end)
            *str = ' ';
        ++str;
    }

    return str;
}

static char *pointer(char *str, const char *end,
                     const char **fmt_ptr,
                     const void *arg, int field_width,
                     int precision,
                     int flags)
{
    const char *fmt = *fmt_ptr;

    switch (fmt[1]) {
    case 'h': /* Raw buffer as hex string. */
    {
        const u8 *hex_buffer = arg;
        char sep = ' '; /* Separator character. */
        unsigned int i;

        /* Consumed 'h' from the format string. */
        ++*fmt_ptr;

        if (field_width <= 0)
            return str;
        if (field_width > 64)
            field_width = 64;

        switch (fmt[2]) {
        case 'C': /* Colons. */
            ++*fmt_ptr;
            sep = ':';
            break;

        case 'D': /* Dashes. */
            ++*fmt_ptr;
            sep = '-';
            break;

        case 'N': /* No separator. */
            ++*fmt_ptr;
            sep = 0;
            break;
        }

        for (i = 0; ; ) {
            /* Each byte: 2 chars, 0-padded, base 16, no hex prefix. */
            str = number(str, end, hex_buffer[i], 16, 2, -1, ZEROPAD);

            if (++i == field_width)
                break;

            if (sep) {
                if ( str < end )
                    *str = sep;
                ++str;
            }
        }

        return str;
    }
    default:
        break;
    }

    if (field_width == -1) {
        field_width = 2 * sizeof(void *);
        flags |= ZEROPAD;
    }

    return number(str, end, (unsigned long)arg,
                  16, field_width, precision, flags);
}

int vsnprint(char *buf, size_t size, const char *fmt, va_list args)
{
    unsigned long long num;
    int base;
    char *str, *end, c;
    const char *s;

    int flags;          /* flags to number() */

    int field_width;    /* width of output field */
    int precision;      /* min. # of digits for integers; max
                           number of chars for from string */
    int qualifier;      /* 'h', 'l', or 'L' for integer fields */
                        /* 'z' support added 23/7/1999 S.H.    */
                        /* 'z' changed to 'Z' --davidm 1/25/99 */

    /* Reject out-of-range values early */
    BUG_ON(((int)size < 0) || ((unsigned int)size != size));

    str = buf;
    end = buf + size;

    if (end < buf) {
        end = ((void *) -1);
        size = end - buf;
    }

    for (; *fmt ; ++fmt) {
        if (*fmt != '%') {
            if (str < end)
                *str = *fmt;
            ++str;
            continue;
        }

        /* process flags */
        flags = 0;
repeat:
        ++fmt;          /* this also skips first '%' */
        switch (*fmt) {
        case '-': flags |= LEFT; goto repeat;
        case '+': flags |= PLUS; goto repeat;
        case ' ': flags |= SPACE; goto repeat;
        case '#': flags |= SPECIAL; goto repeat;
        case '0': flags |= ZEROPAD; goto repeat;
        }

        /* get field width */
        field_width = -1;
        if (isdigit(*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (isdigit(*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;
                          /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
            *fmt =='Z' || *fmt == 'z') {
            qualifier = *fmt;
            ++fmt;
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
                ++fmt;
            }
        }

        /* default base */
        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT)) {
                while (--field_width > 0) {
                    if (str < end)
                        *str = ' ';
                    ++str;
                }
            }
            c = (unsigned char)va_arg(args, int);
            if (str < end)
                *str = c;
            ++str;
            while (--field_width > 0) {
                if (str < end)
                    *str = ' ';
                ++str;
            }
            continue;
        case 's':
            s = va_arg(args, char *);

            str = string(str, end, s, field_width, precision, flags);
            continue;
        case 'p':
            /* pointer() might advance fmt (%pS for example) */
            str = pointer(str, end, &fmt, va_arg(args, const void *),
                          field_width, precision, flags);
            continue;
        case 'n':
            if (qualifier == 'l') {
                long * ip = va_arg(args, long *);
                *ip = (str - buf);
            } else if (qualifier == 'Z' || qualifier == 'z') {
                size_t * ip = va_arg(args, size_t *);
                *ip = (str - buf);
            } else {
                int * ip = va_arg(args, int *);
                *ip = (str - buf);
            }
            continue;
        case '%':
            if (str < end)
                *str = '%';
            ++str;
            continue;
        case 'o':
            base = 8;
            break;
        case 'X':
            flags |= LARGE;
            fallthrough;
        case 'x':
            base = 16;
            break;
        case 'd':
        case 'i':
            flags |= SIGN;
            fallthrough;
        case 'u':
            break;
        default:
            if (str < end)
                *str = '%';
            ++str;
            if (*fmt) {
                if (str < end)
                    *str = *fmt;
                ++str;
            } else {
                --fmt;
            }
            continue;
        }
        if (qualifier == 'L')
            num = va_arg(args, long long);
        else if (qualifier == 'l') {
            num = va_arg(args, unsigned long);
            if (flags & SIGN)
                num = (signed long) num;
        } else if (qualifier == 'Z' || qualifier == 'z') {
            num = va_arg(args, size_t);
        } else if (qualifier == 'h') {
            num = (unsigned short) va_arg(args, int);
            if (flags & SIGN)
                num = (signed short) num;
        } else {
            num = va_arg(args, unsigned int);
            if (flags & SIGN)
                num = (signed int) num;
        }

        str = number(str, end, num, base,
                     field_width, precision, flags);
    }

    if (size > 0) {
        if (str < end)
            *str = '\0';
        else
            end[-1] = '\0';
    }

    return str - buf;
}

int vscnprint(char *buf, size_t size, const char *fmt,
           va_list args)
{
    int i;

    i = vsnprint(buf, size, fmt, args);
    if (i >= size)
        i = size - 1;
    return (i > 0) ? i : 0;
}

int scnprint(char * buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsnprint(buf, size, fmt, args);
    va_end(args);
    if (i >= size)
        i = size - 1;
    return (i > 0) ? i : 0;
}

int snprint(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsnprint(buf, size, fmt, args);
    va_end(args);

    return i;
}

// ####################################################
