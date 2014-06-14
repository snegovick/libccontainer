#include <clist.h>
#include <stdio.h>

/*
Description:

In this example we will:

  1. create list
  2. add few items
  3. enumerate these items

*/

/* This structure describes list item */
struct key {
  char *str;
};

/* This function will be used as an item-destroy callback */
void empty_destroy_function(void *data) {
  return;
}


int main(void) {

/* Step 1: create list */
  struct clist list;
  int ret = clist_init(&list, NULL, empty_destroy_function, CLIST_UNSORTED);
  

/* Step 2: add few items */
  struct key k1 = {"first key"};
  struct key k2 = {"second key"};
  struct key k3 = {"third key"};
  
  clist_append(&list, (void*)&k1);
  clist_append(&list, (void*)&k2);
  clist_append(&list, (void*)&k3);

/* Step 3: enumerate items */
  struct clist_iterator it;
  ret = clist_get_iterator(&list, &it, 0);

  struct key *k;
  ret = clist_iterate(&list, &it, (void **)&k, 0);
  do
  {
    printf("k->str: %s\n", k->str);
  } while (clist_iterate(&list, &it, (void **)&k, 1)==CLIST_TRUE);

/* Step 4: destroy the list */
  clist_destroy(&list);
  
  return 0;
}
