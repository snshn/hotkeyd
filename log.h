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

int open_logs(const char *msg,const char *com){
	if(msg){
		keyd_log=fopen(msg,"w");
		if(!keyd_log){
			log_error("Failed to open log file %s\n",msg);
			return -1;
		}
	}
	if(com){
		com_log=open(com,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
		if(com_log==-1){
			log_error("Failed to open log file %s\n",com);
			return -1;
		}
	}
	return 0;
}

void close_logs(){
	if(keyd_log){
		fclose(keyd_log);
	}
	if(com_log!=-1){
		close(com_log);
	}
}

#endif
