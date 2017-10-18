/** \file scanf.c
  * reduced scanf() added by Andrzej Dworzynski on the base of reduced sscanf() written by
 * some nice gay from this post: http://www.fpgarelated.com/usenet/fpga/show/61760-1.php
 * thanks to everybody:)
 * \date 12.2.2013
 */
#include <stdarg.h>     // (...) parameter handling
#include <stdlib.h>     //NULL pointer definition

//
// Reduced version of sscanf (%d, %x, %c, %n are supported)
// %d dec integer (E.g.: 12)
// %x hex integer (E.g.: 0xa0)
// %b bin integer (E.g.: b1010100010)
// %n hex, de or bin integer (e.g: 12, 0xa0, b1010100010)
// %c any character
//
static int rsscanf(const char* str, const char* format, va_list ap){
    //va_list ap;
    int value, tmp;
    int count;
    int pos;
    char neg, fmt_code;
    const char* pf;
    char* sval;
    //va_start(ap, format);

    for (pf = format, count = 0; *format != 0 && *str != 0; format++, str++){
        while (*format == ' ' && *format != 0) format++;//
        if (*format == 0) break;

        while (*str == ' ' && *str != 0) str++;//increment pointer of input string
        if (*str == 0) break;

        if (*format == '%')//recognize how to format
        {
            format++;
            if (*format == 'n'){
                if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))//if in str sth like 0xff
                {
                    fmt_code = 'x';
                    str += 2;
                } else if (str[0] == 'b') {
                    fmt_code = 'b';
                    str++;
                } else
                    fmt_code = 'd';
            } else
                fmt_code = *format; //it is format letter

            switch (fmt_code){
            case 'x':
            case 'X':
                for (value = 0, pos = 0; *str != 0; str++, pos++){
                    if ('0' <= *str && *str <= '9')
                        tmp = *str - '0';
                    else
                    if ('a' <= *str && *str <= 'f')
                        tmp = *str - 'a' + 10;
                    else
                    if ('A' <= *str && *str <= 'F')
                        tmp = *str - 'A' + 10;
                    else
                        break;

                    value *= 16;
                    value += tmp;
                }
                if (pos == 0)
                    return count;
                *(va_arg(ap, int*)) = value;
                count++;
                break;

            case 'b':
                for (value = 0, pos = 0; *str != 0; str++, pos++){
                    if (*str != '0' && *str != '1')
                        break;
                    value *= 2;
                    value += *str - '0';
                }
                if (pos == 0)
                    return count;
                *(va_arg(ap, int*)) = value;
                count++;
                break;

            case 'd':
                if (*str == '-'){
                    neg = 1;
                    str++;
                } else
                    neg = 0;
                for (value = 0, pos = 0; *str != 0; str++, pos++){
                    if ('0' <= *str && *str <= '9')
                        value = value*10 + (int)(*str - '0');
                    else
                        break;
                }
                if (pos == 0)
                    return count;
                *(va_arg(ap, int*)) = neg ? -value : value;
                count++;
                break;

            case 'c':
                *(va_arg(ap, char*)) = *str;
                count++;
                break;

            case 's':
                sval = va_arg(ap, char*);

                while(*str){
                    *sval++ = *str++;
                    count++;
                }
                *sval = NULL;
                break;

            default:
                return count;
            }
        } else {
            if (*format != *str)//
                break;
        }
    }
    return count;
}

// Reduced version of sscanf (%d, %x, %c, %n are supported)
// %d dec integer (E.g.: 12)
// %x hex integer (E.g.: 0xa0)
// %b bin integer (E.g.: b1010100010)
// %n hex, de or bin integer (e.g: 12, 0xa0, b1010100010)
// %c any character
//buffer support 12 bytes

int sscanf( const char *str, const char* format, ...){
	int count = 0;
    va_list args;
    va_start( args, format );
    count = rsscanf(str, format, args);
    va_end(args);
    return count;
}
