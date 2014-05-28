#include <chash.h>

int chash_init(struct chash_table *table, unsigned int table_size, int (*h)(const void *key, int table_size), int(*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
  int i;
  table->table = (struct clist*)malloc(sizeof(struct clist)*table_size);

  for (i = 0; i < table_size; i++)
  {
    clist_init(&table->table[i], match, destroy, CLIST_UNSORTED);
  }

  table->h = h;
  table->match = match;
  table->destroy = destroy;
  table->buckets = table_size;
  table->size = 0;
  return CHASH_TRUE;
}

int chash_destroy(struct chash_table *table)
{
  int i;
  for (i = 0; i < table->buckets; i++)
  {
    clist_destroy(&table->table[i]);
  }
  free(table->table);
  memset(table, 0, sizeof(struct chash_table));
  return CHASH_TRUE;
}

int chash_insert(struct chash_table *table, const void *data)
{
  void *temp;
  size_t bucket;
  int ret;
  temp = (void *)data;
  if (chash_lookup(table, &temp) == CHASH_TRUE)
  {
    return CHASH_ENOTEMPTY;
  }
  bucket = table->h (data, table->buckets); /* % table->buckets; */
  if ((ret = clist_append(&table->table[bucket], (void*)data)) == CLIST_TRUE)
    table->size ++;
  return ret;
}

int chash_remove(struct chash_table *table, void **data)
{
  struct clist_iterator iterator;
  struct clist_iterator *it = &iterator;
  void *temp;
    
  size_t bucket = table->h(*data, table->buckets); /* % table->buckets; */
    
  clist_get_iterator(&table->table[bucket], it, 0);

  if (clist_iterate(&table->table[bucket], it, (void **)&temp, 0)!=CLIST_TRUE)
  {
    return CHASH_ENOTFOUND;
  }

  do
  {
    if (table->match(*data, temp) == 0)
    {
      if (clist_remove(&table->table[bucket], (void **)&temp, it->id)==CLIST_TRUE)
      {
        table->size--;
        return CHASH_TRUE;
      }
      else
        return CHASH_FALSE;
    }
  }  while (clist_iterate(&table->table[bucket], it, (void **)&temp, 1)==CLIST_TRUE);
  return CHASH_ENOTFOUND;
}

int chash_lookup(struct chash_table *table, void **data)
{
  if (table == NULL)
    return CHASH_FALSE;

  struct clist_iterator iterator;
  struct clist_iterator *it = &iterator;
  void *temp;

  size_t bucket = table->h(*data, table->buckets); /* % table->buckets; */
  
  clist_get_iterator(&table->table[bucket], it, 0);

  if (clist_iterate(&table->table[bucket], it, (void **)&temp, 0)!=CLIST_TRUE)
  {
    return CHASH_ENOTFOUND;
  }
  
  do
  {
    if (table->match(*data, temp) == 0)
    {
      *data = temp;
      return CHASH_TRUE;
    }
  } while (clist_iterate(&table->table[bucket], it, (void **)&temp, 1)==CLIST_TRUE);
  return CHASH_ENOTFOUND;
}

int chash_size(struct chash_table *table)
{
  return table->size;
}

int chash_get_iterator(struct chash_table *table, struct chash_iterator *it)
{
  int ret = 0;
  it->bucket = 0;
  while (((ret = clist_get_iterator(&table->table[it->bucket], &(it->top_level), 0)) != CHASH_TRUE)&& (it->bucket < table->buckets))
    it->bucket ++;
  if (it->bucket >= table->buckets)
      it->bucket = table->buckets-1;
  if (ret != CHASH_TRUE)
    return CHASH_FALSE;

  ret = clist_iterate(&table->table[it->bucket], &(it->top_level), (void **)&(it->lptr), 0);
  if (ret != CLIST_TRUE)
    return CHASH_FALSE;

  return CHASH_TRUE;
}

int chash_iterate(struct chash_table *table, struct chash_iterator *it, void **data)
{
  int ret1 = clist_iterate(&table->table[it->bucket], &(it->top_level), data, 0);
  if (ret1 != CLIST_TRUE)
    return CHASH_LAST;
  int ret = clist_iterate(&table->table[it->bucket], &(it->top_level), NULL, 1);
  if (ret != CLIST_TRUE)
  {
    do {
      it->bucket ++;
    } while ((it->bucket < table->buckets) && ((ret = clist_get_iterator(&table->table[it->bucket], &(it->top_level), 0)) != CLIST_TRUE));
    
    if (it->bucket >= table->buckets)
      it->bucket = table->buckets-1;

    if (ret != CLIST_TRUE)
    {
      if (ret1 == CLIST_TRUE)
        return CHASH_LAST;
      return CHASH_FALSE;
    }
    ret = clist_iterate(&table->table[it->bucket], &(it->top_level), NULL, 0);
    if (ret != CLIST_TRUE)
      return CHASH_LAST;
  }
  return CHASH_TRUE;
}

int str_hash_function(const void *key, int table_size)
{
  const char *ptr;
  int val;
  val = 0;
  struct basic_key *k = (struct basic_key *)key;
  ptr = k->str;
  while (*ptr != '\0')
  {
    size_t tmp;
    val = (val << 4) + (*ptr);
    tmp = (val & 0xf0000000);
    if (tmp) {
      val = val ^ (tmp >> 24);
      val = val ^ tmp;
    }
    ptr++;
  }
  return val % table_size;
}

int str_match_function(const void* key1, const void *key2)
{
  struct basic_key *k1 = (struct basic_key *)key1;
  struct basic_key *k2 = (struct basic_key *)key2;
  return strcmp(k1->str, k2->str);
}
