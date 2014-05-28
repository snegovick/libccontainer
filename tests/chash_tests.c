#include "CUnit/Basic.h"
#include <chash.h>
#include <stdio.h>

int init_chash_test_suite(void)
{
  return 0;
}

int clean_chash_test_suite(void)
{
  return 0;  
}

struct key
{
  char *str;
  int id;
};

int int_hash_function(const void *key, int table_size)
{
  const char *ptr;
  int val;
  val = 0;
  ptr = key;
  while (*ptr != '\0')
  {
    int tmp;
    val = (val << 4) + (*ptr);
    tmp = (val & 0xf0000000);
    if (tmp) {
      val = val ^ (tmp >> 24);
      val = val ^ tmp;
    }
    ptr++;
  }
  return val;
}


int str_hash_function(const void *key, int table_size)
{
  const char *ptr;
  int val;
  val = 0;
  struct key *k = (struct key *)key;
  ptr = k->str;
  printf("hashing %s; ", k->str);
  while (*ptr != '\0')
  {
    int tmp;
    val = (val << 4) + (*ptr);
    tmp = (val & 0xf0000000);
    if (tmp) {
      val = val ^ (tmp >> 24);
      val = val ^ tmp;
    }
    ptr++;
  }
  printf("hash: %i\n", val % table_size);
  return val % table_size;
}

int str_match_function(const void* key1, const void *key2)
{
  struct key *k1 = (struct key*)key1;
  struct key *k2 = (struct key*)key2;
  printf("matching %s against %s: %i\n", k1->str, k2->str, strcmp(k1->str, k2->str));
  return strcmp(k1->str, k2->str);
}

void empty_destroy_function(void *data)
{
  return;
}

void free_destroy_function(void *data)
{
  free(data);
  return;
}

#define TABLE_SIZE 10
void test_chash_insert(void)
{
  struct key k1;
  struct key k2;
  struct key k3;

  k1.str = "key1";
  k2.str = "key2";
  k3.str = "key3";

  struct chash_table t;
  chash_init(&t, TABLE_SIZE, str_hash_function, str_match_function, empty_destroy_function);
  
  CU_ASSERT(chash_insert(&t, (void*)&k1) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k2) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k3) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k1) != CHASH_TRUE);
  printf("table->size: %i\n", t.size);

}

void test_chash_remove(void)
{
  struct key k1;
  struct key k2;
  struct key k3;

  k1.str = "key1";
  k1.id = 10;
  k2.str = "key2";
  k2.id = 20;
  k3.str = "key3";
  k3.id = 30;

  struct chash_table t;
  chash_init(&t, TABLE_SIZE, str_hash_function, str_match_function, empty_destroy_function);
  
  CU_ASSERT(chash_insert(&t, (void*)&k2) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k1) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k3) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);

/* this key should be found */
  struct key lookup;
  lookup.str = "key1";
  struct key * kptr = &lookup;

  CU_ASSERT(chash_lookup(&t, (void **)&kptr) == CHASH_TRUE);
  printf("kptr->id: %i\n", kptr->id);
  CU_ASSERT(kptr->id==10);

/* this key should be found too */
  lookup.str = "key2";
  kptr = &lookup;

  CU_ASSERT(chash_lookup(&t, (void **)&kptr) == CHASH_TRUE);
  printf("kptr->id: %i\n", kptr->id);
  CU_ASSERT(kptr->id==20);

  printf("removing key2\n");
  lookup.str = "key2";
  kptr = &lookup;
  CU_ASSERT(chash_remove(&t, (void **)&kptr) == CHASH_TRUE);
  printf("removed\n");

  printf("removing key2 again\n");
  lookup.str = "key2";
  kptr = &lookup;
  CU_ASSERT(chash_remove(&t, (void **)&kptr) != CHASH_TRUE);
  printf("removed\n");

/* this key should not be found */
  lookup.str = "key2";
  kptr = &lookup;
  CU_ASSERT(chash_lookup(&t, (void **)&kptr) != CHASH_TRUE);
}

void test_chash_lookup(void)
{
  struct key k1;
  struct key k2;
  struct key k3;

  k1.str = "key1";
  k1.id = 10;
  k2.str = "key2";
  k2.id = 20;
  k3.str = "key3";
  k3.id = 30;

  struct chash_table t;
  chash_init(&t, TABLE_SIZE, str_hash_function, str_match_function, empty_destroy_function);
  
  CU_ASSERT(chash_insert(&t, (void*)&k2) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k1) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k3) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);

/* this key should be found */
  struct key lookup;
  lookup.str = "key1";
  struct key * kptr = &lookup;

  CU_ASSERT(chash_lookup(&t, (void **)&kptr) == CHASH_TRUE);
  printf("kptr->id: %i\n", kptr->id);
  CU_ASSERT(kptr->id==10);

/* this key should be found too */
  lookup.str = "key2";
  kptr = &lookup;

  CU_ASSERT(chash_lookup(&t, (void **)&kptr) == CHASH_TRUE);
  printf("kptr->id: %i\n", kptr->id);
  CU_ASSERT(kptr->id==20);


  /* this operation should fail */
  CU_ASSERT(chash_insert(&t, (void*)&k1) != CHASH_TRUE);
  printf("table->size: %i\n", t.size);


/* this key should not be found */
  lookup.str = "key10";
  kptr = &lookup;
  CU_ASSERT(chash_lookup(&t, (void **)&kptr) != CHASH_TRUE);

  CU_ASSERT(chash_lookup(NULL, (void **)&kptr) == CHASH_FALSE);
}

void test_chash_traverse(void)
{
  struct key k1;
  struct key k2;
  struct key k3;

  k1.str = "key1";
  k1.id = 10;
  k2.str = "key2";
  k2.id = 20;
  k3.str = "key3";
  k3.id = 30;

  struct chash_table t;
  chash_init(&t, TABLE_SIZE, str_hash_function, str_match_function, empty_destroy_function);
  
  CU_ASSERT(chash_insert(&t, (void*)&k2) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k1) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)&k3) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);

  struct chash_iterator it;
  int ret = chash_get_iterator(&t, &it);
  CU_ASSERT(ret == CHASH_TRUE);
  
  if (ret != CHASH_TRUE)
    return;
  int ctr = 0;
  struct key *tkey;
  int lret = CHASH_TRUE;
  while((lret == CHASH_TRUE))
  {
    lret = chash_iterate(&t, &it, (void **)&tkey);
    printf("%i: key id[%i]: %s\n", ctr, tkey->id, tkey->str);
    ctr ++;

  }
  CU_ASSERT(ctr == 3);
}

void test_chash_destroy(void)
{
  struct key *k1 = malloc(sizeof(struct key));
  struct key *k2 = malloc(sizeof(struct key));
  struct key *k3 = malloc(sizeof(struct key));

  k1->str = "key1";
  k1->id = 10;
  k2->str = "key2";
  k2->id = 20;
  k3->str = "key3";
  k3->id = 30;

  struct chash_table t;
  chash_init(&t, TABLE_SIZE, str_hash_function, str_match_function, empty_destroy_function);
  
  CU_ASSERT(chash_insert(&t, (void*)k2) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)k1) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);
  CU_ASSERT(chash_insert(&t, (void*)k3) == CHASH_TRUE);
  printf("table->size: %i\n", t.size);

  CU_ASSERT(chash_destroy(&t) == CHASH_TRUE);  
}

int main()
{
  CU_pSuite pSuite = NULL;
  
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  
  pSuite = CU_add_suite("chash suite", init_chash_test_suite, clean_chash_test_suite);
  if (NULL == pSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_chash_insert))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_chash_remove))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_chash_lookup))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_chash_destroy))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_chash_traverse))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  printf ("running tests\n");
  /* CU_automated_run_tests(); */
  CU_cleanup_registry();
  return CU_get_error();
}
