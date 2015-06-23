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
#ifndef HEADER_NVLIST_UTIL_H
#define HEADER_NVLIST_UTIL_H
#define NVLIST_UTIL_COLOR_RED "\x1b[31m"
#define NVLIST_UTIL_COLOR_GREEN "\x1b[32m"
#define NVLIST_UTIL_COLOR_YELLOW "\x1b[33m"
#define NVLIST_UTIL_COLOR_BLUE "\x1b[34m"
#define NVLIST_UTIL_COLOR_MAGENTA "\x1b[35m"
#define NVLIST_UTIL_COLOR_CYAN "\x1b[36m"
#define NVLIST_UTIL_COLOR_RESET "\x1b[0m"
#define NVLIST_UTIL_NL "\n"

/*
 * deb() print string with color
 * for debugging purposes
 */
void deb(char* msg, char* color);

/*
 * get_exe_path() return current application path
 * if full_path is true, it will return full path
 * otherwise just application name
 */
char *get_exe_path();

/*
 * get_exe_database_path() return current sqlite file path
 * /home/<user>/.config/<appname><id provided in the parameters>
 */
char *get_exe_database_path(const char *id);


#endif /* HEADER_NVLIST_UTIL_H */

