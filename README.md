# libnvlist
C library thar provides Name-Value linked list with SQLite persistence.

# Dependencies:
- libsqlite3

# Usage:
```C
#include <stdio.h>
#include <nvlist.h>

int main(int argc, char **argv)
{
	nvlist *list = NULL;
	list = nvlist_set(list, "name", "Turok");
	list = nvlist_set(list, "color", "Blue");
	nvlist_view(list);
	
	/* PRINTOUT:
	name - Turok
	color - Blue
	*/
	
	list = nvlist_set(list, "color", "Red");
	nvlist_view(list);
	
	/* PRINTOUT:
	name - Turok
	color - Red
	*/
	
	char *name = nvlist_get(list, "name");
	printf("Name: %s\n", name);
	
	/* PRINTOUT:
	Name: Turok
	*/
	
	int delete_result;
	list = nvlist_delete(list, "color", &delete_result);
	if (delete_result) {
		printf("color removed\n");
	}
	nvlist_view(list);
	/* PRINTOUT:
	name - Turok
	*/
	
	if (nvlist_persist(list, "my_users") == NVLIST_OK) {
		printf("persisted\n");
	}
	// The list will be stored in SQLITE database in your /home/<user>/.config/<yourappname>my_users.db file
	
	// After restart your application it can be retrieved
	if (nvlist_retrieve(&list, "my_users") == NVLIST_OK) {
		printf("loaded ok\n");
	}
	nvlist_view(list);
	
	/* PRINTOUT:
	name - Turok
	*/
	
	nvlist_free_all(list); // don't forget to free all resources
	
	return 0;
}

```

