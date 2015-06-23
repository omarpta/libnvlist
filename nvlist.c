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
#include "nvlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>

/*
 * name_value_list_view() show current list elements
 * for debugging purposes
 */
void name_value_list_view(nvlist *list) {
	nvlist* next;
    nvlist* item;

    if(!list)
	return;

    deb("Current Name List Items:\n", NVLIST_UTIL_COLOR_BLUE);

    item = list;
    do {
		next = item->next;
		if (item->data) {
			if (item->data->name && item->data->value) {
				deb(item->data->name, NVLIST_UTIL_COLOR_BLUE);
				deb(" - ", NVLIST_UTIL_COLOR_BLUE);
				deb(item->data->value, NVLIST_UTIL_COLOR_BLUE);
				deb("\n", NVLIST_UTIL_COLOR_RESET);
				
			}
		}
		item = next;
    } while(next);
}

/*
 * name_value_list_get_last() get the last item from list
 * return NULL if list is empty
 */
nvlist *name_value_list_get_last(nvlist *list) {
  nvlist     *item;

  /* if caller passed us a NULL, return now */
  if(!list)
    return NULL;

  /* loop through to find the last item */
  item = list;
  while(item->next) {
    item = item->next;
  }
  return item;
}

/*
 * name_value_list_add_nodup() add item to the list
 * return NULL if heap allocation fail
 */
nvlist *name_value_list_add_nodup(nvlist *list, char *name, char *value)
{
  nvlist     *last;
  nvlist     *new_item;
  nvitem 	 *data;

  new_item = malloc(sizeof(nvlist));
  if(!new_item)
    return NULL;

  new_item->next = NULL;
  data = (nvitem *) malloc(sizeof(nvitem));
  
  data->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
  strcpy(data->name, name);
  data->value = (char*) malloc((strlen(value) + 1) * sizeof(char));
  strcpy(data->value, value);
  new_item->data = (nvitem *) malloc(sizeof(nvitem));
  new_item->data = data;

  /* if this is the first item, then new_item *is* the list */
  if(!list)
    return new_item;

  last = name_value_list_get_last(list);
  last->next = new_item;
  
  return list;
}

/*
 * name_value_list_set() set or add item to the list
 * if item already exists, it will be updated otherwise will be 
 * added to the list
 * return NULL if name or value char duplication fail
 */
nvlist *name_value_list_set(nvlist *list, const char* name, const char* value) {
	int found = 0;
	char *dupname = strdup(name);
	char *dupvalue = strdup(value);

	if(!dupname)
		return NULL;

	if(!dupvalue)
		return NULL;

	if (list) {
		nvlist* next;
		nvlist* item;

		item = list;
		do {
			next = item->next;
			if (item) {
				if (item->data->name && item->data->value) {
					if (strstr(item->data->name, name) != NULL) {
						found = 1;
						free(item->data->value);
						//item->data->value = (char *)realloc(item->data->value, (strlen(dupvalue) + 1) * sizeof(char));
						item->data->value = (char *)malloc((strlen(dupvalue) + 1) * sizeof(char));
						item->data->value = dupvalue;
					}
				}
			}
			item = next;
		} while(next);	
	}

	if (!found) {
		list = name_value_list_add_nodup(list, dupname, dupvalue);
	}
	if(!list) {
		free(dupname);
		free(dupvalue);
	}	

	return list;
}

/*
 * name_value_list_get() get item from list by its name
 * return NULL if list is empty or item does not exists
 */
char *name_value_list_get(nvlist *list, const char* name) {
	nvlist* next;
    nvlist* item;

    if(!list)
	return NULL;

    item = list;
    do {
		next = item->next;
		if (item) {
			if (item->data->name && item->data->value) {
				if (strstr(item->data->name, name) != NULL) {
					return item->data->value;
				}
			}
		}
		item = next;
    } while(next);	
	return NULL;
}

/*
 * name_value_list_delete() delete item from list
 * return NULL if list is empty
 */
nvlist *name_value_list_delete(nvlist *list, char* name, int* ok) {
	nvlist* next;
    nvlist* item;
	nvlist* last = NULL;


    if(!list)
	return NULL;

    item = list;
    do {
		//printf("Verificando %s\n",item->data->name);
		next = item->next;
		if (item->data) {
			if (item->data->name && item->data->value) {
				if (strstr(item->data->name, name) != NULL) {
					if (last) {
						last->next = item->next;
					} 
					free(item);
					
					*ok = 1;
					break;
				}
			}
		}
		last = item;
		item = next;
    } while(next);
	if (last == NULL) {
		return next;
	}
	return list;
	
}

/*
 * name_value_list_free_all() free list and all
 * allocated memory
 */
void name_value_list_free_all(nvlist *list)
{
  nvlist     *next;
  nvlist     *item;

  if(!list)
    return;

  item = list;
  do {
    next = item->next;
    free(item->data->name);
	free(item->data->value);
	free(item->data);
    free(item);
    item = next;
  } while(next);
}

/*
 * name_value_list_persist() persist list in sqlite file 
 * /home/<user>/.config/<appname><id provided in the parameters>
 */
int name_value_list_persist(nvlist *list, const char* id) {
	char *full_database_path = get_exe_database_path(id);	
	
	
	sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open(full_database_path, &db);
    
    if (rc != SQLITE_OK) {
        
        //fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return NAME_VALUE_ERROR_CANNOT_OPEN;
    }
    

	nvlist* next;
	nvlist* item;

	if(!list)
	return NAME_VALUE_ERROR_LIST_NULL;

    char *sql_create = "DROP TABLE IF EXISTS nvlist; CREATE TABLE nvlist(id INTEGER PRIMARY KEY, name TEXT, value TEXT);";
	char *sql_insert = " INSERT INTO nvlist (name,value) VALUES('','');";
	int sql_create_len = strlen(sql_create);
	int sql_insert_len = strlen(sql_insert);
	int len_data = 0;
	char *sql = (char*) malloc((sql_create_len + 1) * sizeof(char));
	strcpy(sql, sql_create);
	item = list;
	do {
		next = item->next;
		if (item->data) {
			if (item->data->name && item->data->value) {
				len_data = strlen(sql) + strlen(item->data->name) + strlen(item->data->value) + sql_insert_len +1;
				sql = (char*) realloc(sql, len_data * sizeof(char));
				strcat(sql, " INSERT INTO nvlist (name,value) VALUES('");
				strcat(sql, item->data->name);
				strcat(sql, "','");
				strcat(sql, item->data->value);
				strcat(sql, "');");
			}
		}
		item = next;
	} while(next);
	
	
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return NAME_VALUE_ERROR_CANNOT_PERSIST;
    } 
    
    sqlite3_close(db);
	free(sql);
	free(full_database_path);
    return NAME_VALUE_OK;
}


/*
 * name_value_list_retrieve() retrieve previous persisted list from 
 * /home/<user>/.config/<appname><id provided in the parameters>
 */
int name_value_list_retrieve(nvlist **list, const char* id) {
	char *full_database_path = get_exe_database_path(id);	
	sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open(full_database_path, &db);
    
    if (rc != SQLITE_OK) {
        
        //fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return NAME_VALUE_ERROR_CANNOT_OPEN;
    }
	
	rc = sqlite3_prepare_v2(db, "select * from nvlist", -1, &stmt, NULL);
	if(rc!=SQLITE_OK) {
			//fprintf(stderr, "sql error #%d: %s\n", rc, sqlite3_errmsg(db));
			return NAME_VALUE_ERROR_EMPTY;
	} else while((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
			switch(rc) {
			  case SQLITE_BUSY:
					  //fprintf(stderr, "busy, wait 1 seconds\n");
					  //break;
					  return NAME_VALUE_NOT_OK;
			  case SQLITE_ERROR:
					  //fprintf(stderr, "step error: %s\n", sqlite3_errmsg(db));
					  //break;
					  return NAME_VALUE_NOT_OK;
			  case SQLITE_ROW:
					  {
							  /*int n = sqlite3_column_count(stmt);
							  int i;
							  for(i=0; i<n; i++) {
									  printf("%s = ", sqlite3_column_name(stmt, i));
									  switch(sqlite3_column_type(stmt, i)) {
									  case SQLITE_TEXT:
											  printf("%s", sqlite3_column_text(stmt, i));
											  break;
									  case SQLITE_INTEGER:
											  printf("%d", sqlite3_column_int(stmt, i));
											  break;
									  case SQLITE_FLOAT:
											  printf("%f", sqlite3_column_double(stmt, i));
											  break;
									  case SQLITE_BLOB:
											  printf("(blob)");
											  break;
									  case SQLITE_NULL:
											  printf("(null)");
											  break;
									  default:
											  printf("(unknown: %d)", sqlite3_column_type(stmt, i));
									  }
									  printf("\n");
							  }*/
							  const  char *name = (char*) sqlite3_column_text(stmt, 1);
							  const  char *value = (char*) sqlite3_column_text(stmt, 2);
							  //printf("DATA: %s - %s <> %zu, %zu\n", name, value, strlen(name), strlen(value));
							  *list = name_value_list_set(*list, name, value);
					  }
					  break;
			  }
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	free(full_database_path);
	return NAME_VALUE_OK;
}