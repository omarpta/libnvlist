/***************************************************************************
*         _   ___      ___      _____  _____ _______ 
*        | \ | \ \    / / |    |_   _|/ ____|__   __|
 *       |  \| |\ \  / /| |      | | | (___    | |   
 *       | . ` | \ \/ / | |      | |  \___ \   | |   
 *       | |\  |  \  /  | |____ _| |_ ____) |  | |   
 *       |_| \_|   \/   |______|_____|_____/   |_|   
                                             
 *
 * Copyright (C) 2015, Omar Torres, <omar@analista.io>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://nvlist.analista.io/copyright.html .
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
 
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * deb() print string with color
 * for debugging purposes
 */
void deb(char* msg, char* color)
{
    printf("%s%s" NVLIST_UTIL_COLOR_RESET, color, msg);
}

/*
 * get_exe_path() return current application path
 * if full_path is true, it will return full path
 * otherwise just application name
 */
char *get_exe_path(int full_path) {
	char exe[1024];
    int ret;

    ret = readlink("/proc/self/exe",exe,sizeof(exe)-1);
    if(ret ==-1) {
        return NULL;
    }
    exe[ret] = 0;
    char *exe_name;
	exe_name = (char*)malloc((strlen(exe) + 1) * sizeof(char));
	strcpy(exe_name, exe);
	
	if (full_path) {
		return exe_name;
	} else {
		char *part;
		char *last_part = NULL;
		part = strtok(exe_name, "/");
		while( part != NULL ) 
		{
			if (last_part) {
				free(last_part);
			}
			last_part = (char*)malloc((strlen(part) + 1) * sizeof(char));
			strcpy(last_part, part);
			part = strtok(NULL, "/");
		}
		return last_part;
	}

}

/*
 * get_exe_database_path() return current sqlite file path
 * /home/<user>/.config/<appname><id provided in the parameters>
 */
char *get_exe_database_path(const char *id) {
	char *home = getenv("HOME");
	char *config_path = "/.config/";
	char *dbname = ".db";
	char *exe_name = get_exe_path(0);
	char *full_database_path;
	int full_database_path_len = strlen(home) + strlen(config_path) + strlen(exe_name)  + strlen(id) +strlen(dbname)+ 1;
	
	full_database_path = (char*) malloc(full_database_path_len * sizeof(char));
	if (!full_database_path)
		return NULL;
		
	strcpy(full_database_path, home);
	strcat(full_database_path, config_path);
	strcat(full_database_path, exe_name);
	strcat(full_database_path, id);
	strcat(full_database_path, dbname);
	
	free(exe_name);
	return full_database_path;
}