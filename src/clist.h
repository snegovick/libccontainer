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

/*
  match returns -1 if k1<k2 0 if k1 == k2, 1 if k1>k2
*/
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

int clist_init(struct clist *list, int (*match)(const void *key1, const void* key2), void (*destroy)(void *data), unsigned int sorted);
int clist_get_by_id(struct clist *list, void **data, int id);
int clist_destroy(struct clist *list);
int clist_append(struct clist *list, void *data);
int clist_pop(struct clist *list);
int clist_remove(struct clist *list, void **data, int id);
int clist_insert(struct clist *list, void *data, int id);
int clist_get_iterator(struct clist *list, struct clist_iterator *it, int start);
int clist_iterate(struct clist *list, struct clist_iterator *it, void **data, int step);
int clist_post_iterate(struct clist *list, struct clist_iterator *it, void **data, int step);
int clist_qsort(struct clist *list, enum sort_order order);

#endif/*__CLIST_H__*/
