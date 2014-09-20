#ifndef _INPUT_DEV_H
#define _INPUT_DEV_H

#include <dirent.h>

int is_keyboard(const char *name){
	int l=strlen(name);
	if(strncmp("-kbd",name+l-4,4)==0){
		return 1;
	}
	return 0;
}

char *default_device(){
	DIR *d;
	struct dirent *ent;
	char *tret=NULL;
	d=opendir("/dev/input/by-id");
	while(ent=readdir(d)){
		if(is_keyboard(ent->d_name)){
			tret=(char*)malloc(sizeof(char)*(18+strlen(ent->d_name)));
			if(tret){
				tret[0]=0;
				strcat(tret,"/dev/input/by-id/");
				strcat(tret,ent->d_name);
			}
			break;
		}
	}
	closedir(d);
	if(tret){
		return tret;
	}
	d=opendir("/dev/input/by-path");
	while(ent=readdir(d)){
		if(is_keyboard(ent->d_name)){
			tret=(char*)malloc(sizeof(char)*(20+strlen(ent->d_name)));
			if(tret){
				tret[0]=0;
				strcat(tret,"/dev/input/by-path/");
				strcat(tret,ent->d_name);
			}
			break;
		}
	}
	closedir(d);
	if(tret){
		return tret;
	}
	return NULL;
}

#endif
