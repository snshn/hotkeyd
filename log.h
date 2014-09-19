/*
	keyd.c
	
	developed by benjamin bolton
	https://bennybolton.com/
	license: GPLv3
*/

#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h>
#include <stdio.h>

FILE *keyd_log=NULL;
int com_log=-1;

void log_msg(const char *text,...){
	va_list args;
	va_start(args,text);
	vfprintf(stdout,text,args);
	va_start(args,text);
	if(keyd_log){
		vfprintf(keyd_log,text,args);
	}
	va_end(args);
}

void log_error(const char *text,...){
	va_list args;
	va_start(args,text);
	vfprintf(stderr,text,args);
	va_start(args,text);
	if(keyd_log){
		vfprintf(keyd_log,text,args);
	}
	va_end(args);
}

#endif
