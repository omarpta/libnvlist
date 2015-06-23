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

#ifndef HEADER_NVLIST_H
#define HEADER_NVLIST_H


/*
 * MESSAGE CODES
 */
#define NVLIST_NOT_OK 0
#define NVLIST_OK 1
#define NVLIST_ERROR_CANNOT_OPEN 2
#define NVLIST_ERROR_LIST_NULL 3
#define NVLIST_ERROR_CANNOT_PERSIST 4
#define NVLIST_ERROR_EMPTY 5

/*
 * NVLIST ITEM structure
 */
typedef struct nvlist_item {
	char *name;
	char *value;
} nvitem;

/*
 * NVLIST ITEM linked list structure
 */
typedef struct nvlist_list {
  nvitem *data;
  struct nvlist_list *next;
} nvlist;

/*
 * nvlist_list_view() show current list elements
 * for debugging purposes
 */
extern void nvlist_view(nvlist *list);

/*
 * nvlist_list_get_last() get the last item from list
 * return NULL if list is empty
 */
extern nvlist *nvlist_get_last(nvlist *list);

/*
 * nvlist_list_add_nodup() add item to the list
 * return NULL if heap allocation fail
 */
nvlist *nvlist_add_nodup(nvlist *list, char *name, char *value);

/*
 * nvlist_set() set or add item to the list
 * if item already exists, it will be updated otherwise will be 
 * added to the list
 * return NULL if name or value char duplication fail
 */
extern nvlist *nvlist_set(nvlist *list, const char* name, const char* value);										

/*
 * nvlist_get() get item from list by its name
 * return NULL if list is empty or item does not exists
 */
extern char *nvlist_get(nvlist *list, const char* name);

/*
 * nvlist_delete() delete item from list
 * return NULL if list is empty
 */
extern nvlist *nvlist_delete(nvlist *list, char* name, int* ok);

/*
 * nvlist_free_all() free list and all
 * allocated memory
 */
extern void nvlist_free_all(nvlist *list);

/*
 * nvlist_persist() persist list in sqlite file 
 * /home/<user>/.config/<appname><id provided in the parameters>
 */
extern int nvlist_persist(nvlist *list, const char* id);

/*
 * nvlist_retrieve() retrieve previous persisted list from 
 * /home/<user>/.config/<appname><id provided in the parameters>
 */
extern int nvlist_retrieve(nvlist **list, const char* id);
										
#endif /* HEADER_NVLIST_H */