#ifndef __CHASH_H__
#define __CHASH_H__

#include "clist.h"

#define CHASH_TRUE       1
#define CHASH_FALSE      0
#define CHASH_ENOTFOUND -1
#define CHASH_ENOTEMPTY -2
#define CHASH_LAST       2

struct chash_table
{
  int buckets;
  int (*h)(const void *key, int size);
  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  int size;
  struct clist *table;
};

struct basic_key
{
  char *str;
  void *data;
};

struct chash_iterator
{
  struct clist_iterator top_level;
  struct clist_iterator low_level;
  struct clist *lptr;
  size_t bucket;
};


int chash_init(struct chash_table *table, unsigned int table_size, int (*h)(const void *key, int table_size), int(*match)(const void *key1, const void *key2), void (*destroy)(void *data));
int chash_destroy(struct chash_table *table);
int chash_insert(struct chash_table *table, const void *data);
int chash_remove(struct chash_table *table, void **data);
int chash_lookup(struct chash_table *table, void **data);
int chash_size(struct chash_table *table);
int chash_get_iterator(struct chash_table *table, struct chash_iterator *it);
int chash_iterate(struct chash_table *table, struct chash_iterator *it, void **data);
int str_hash_function(const void *key, int table_size);
int str_match_function(const void* key1, const void *key2);

#endif/*__CHASH_H__*/
