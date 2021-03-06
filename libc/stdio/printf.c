#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static int print(const char* data, size_t length)
{
    const unsigned char* bytes = (const unsigned char*) data;

    for (size_t i = 0; i < length; i++)
    {
        if (putchar(bytes[i]) == EOF) return 0;
    }

    return 1;
}

int printf(const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0')
    {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%') format ++;

            size_t amount = 1;

            while (format[amount] && format[amount] != '%') amount ++;

            if (maxrem < amount)
            {
                return -1;
            }

            if (!print(format, amount)) return -1;

            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        if (*format == 'c')
        {
            format++;
            char c = (char) va_arg(parameters, int);

            if (!maxrem)
            {
                return -1;
            }

            if (!print(&c, sizeof(c))) return -1;

            written++;
        }
        else if (*format == 's')
        {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);

            if (maxrem < len)
            {
                return -1;
            }

            if (!print(str, len)) return -1;
            written += len;
        }
        else if (*format == 'd')
        {
            //for now only works with positive numbers
            format++;

            int i = va_arg(parameters, int);
            int il = i;
            size_t len = 0;

            while (il)
            {
                len++;
                il /= 10;
            }

            if (i == 0)
            {
                len = 1;
            }

            if (maxrem < len)
            {
                return -1;
            }

            char digits[len];

            size_t lenc = len;
            while (lenc--)
            {
                digits[lenc] = (i % 10) + '0';
                i /= 10;
            }

            if (!print(digits, len)) return -1;

            written += len;
        }
        else
        {
            format = format_begun_at;
            size_t len = strlen(format);

            if (maxrem < len)
            {
                return -1;
            }

            if (!print(format, len)) return -1;

            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}
