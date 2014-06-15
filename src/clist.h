#ifndef __CLIST_H__
#define __CLIST_H__

#include <clist_common.h>
#include <stdlib.h>
#include <string.h>

struct clist_node;

struct clist_node
{
  void *data;
  struct clist_node *next;
  struct clist_node *prev;
};

struct clist
{
  struct clist_node *head;
  struct clist_node *tail;
  unsigned int size;
  unsigned int sorted;
  int (*match)(const void *key1, const void* key2);
  void (*destroy)(void *data);
};

struct clist_iterator
{
  struct clist_node *n;
  int id;
};

enum sort_order {
  ASCENDING,
  DESCENDIN
};

#define CLIST_SORTED 1
#define CLIST_UNSORTED 0

/**
   Init function.
   @param match is a comparison callback used for sorting and searching; match returns -1 if k1<k2 0 if k1 == k2, 1 if k1>k2
   @param destroy is a destroy callback used for freeing elements when pop is called;
   @param sorted is not used for now;
*/
int clist_init(struct clist *list, int (*match)(const void *key1, const void* key2), void (*destroy)(void *data), unsigned int sorted);

/**
   Get data by id of element.
   @param data will be pointing to the found elements data field;
   @param id should be a value from 0 to list size-1;
 */
int clist_get_by_id(struct clist *list, void **data, int id);

/**
   Does nothing for now, but call after you are done with the list for compatibility with future versions.
 */
int clist_destroy(struct clist *list);

/**
   Append the data to the end of the list
 */
int clist_append(struct clist *list, void *data);

/**
   Remove the last element of the list.
 */
int clist_pop(struct clist *list);

/**
   Remove element with specified id;
   @param data will be pointing to elements data;
 */
int clist_remove(struct clist *list, void **data, int id);

/**
   Insert element at specified place;
 */
int clist_insert(struct clist *list, void *data, int id);

/**
   Initializes iterator,
   @param id can take any value from -list->size, to + list->size. Negative value means "count from tail". Positive values - count from head;
 */
int clist_get_iterator(struct clist *list, struct clist_iterator *it, int start);

/**
   Move iterator front or back step elements, then return data value.
   @param data is used to return element data value;
   @param step can be positive or negative;
 */
int clist_iterate(struct clist *list, struct clist_iterator *it, void **data, int step);

/**
   Set data value, then move iterator front or back step elements.
   @param data is used to return elemnt data value;
   @param step can be positive or negative;
 */
int clist_post_iterate(struct clist *list, struct clist_iterator *it, void **data, int step);

/**
   Sort the list inplace.
   @param order is not working now. The order is ascending;
 */
int clist_qsort(struct clist *list, enum sort_order order);

#endif/*__CLIST_H__*/
