/*
    keyd.c

    Forked from Benjamin Bolton's keyd
    license: GPLv3
*/

#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h>
#include <stdio.h>

void log_msg(const char *text, ...)
{
    va_list args;
    va_start(args, text);
    vfprintf(stdout, text, args);
    va_start(args, text);
    va_end(args);
}

void log_err(const char *text, ...)
{
    va_list args;
    va_start(args, text);
    vfprintf(stderr, text, args);
    va_start(args, text);
    va_end(args);
}

#endif
