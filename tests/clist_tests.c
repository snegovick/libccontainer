#include "CUnit/Basic.h"
#include <clist.h>
#include <stdio.h>

int init_clist_test_suite(void)
{
  return 0;
}

int clean_clist_test_suite(void)
{
  return 0;  
}

struct key
{
  int id;
};

int match_function(const void* key1, const void *key2)
{
  struct key *k1 = (struct key*)key1;
  struct key *k2 = (struct key*)key2;
  if (k1->id > k2->id)
    return 1;
  if (k1->id < k2->id)
    return -1;
  return 0;
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

void test_clist_append_insert_append(void)
{
#define NUM_ELEMENTS 10
  struct clist list;
  int ret = clist_init(&list, match_function, free_destroy_function, CLIST_UNSORTED);
  struct key *k = NULL;
  int ctr = NUM_ELEMENTS;
  for (; ctr>0; ctr--)
  {
    k = (struct key*)malloc(sizeof(struct key));
    k->id = ctr;
    printf("adding id: %i\n", k->id);
    ret = clist_append(&list, (void *)k);
    CU_ASSERT(ret==CLIST_TRUE);
  }
  for (ctr=NUM_ELEMENTS; ctr>0; ctr--)
  {
    k = (struct key*)malloc(sizeof(struct key));
    k->id = ctr;
    printf("adding id: %i\n", k->id);
    ret = clist_insert(&list, (void *)k, ctr);
    CU_ASSERT(ret==CLIST_TRUE);
  }
  for (ctr=NUM_ELEMENTS; ctr>0; ctr--)
  {
    k = (struct key*)malloc(sizeof(struct key));
    k->id = ctr;
    printf("adding id: %i\n", k->id);
    ret = clist_insert(&list, (void *)k, 0);
    CU_ASSERT(ret==CLIST_TRUE);
  }
  for (ctr=NUM_ELEMENTS; ctr>0; ctr--)
  {
    k = (struct key*)malloc(sizeof(struct key));
    k->id = ctr;
    printf("adding id: %i\n", k->id);
    ret = clist_append(&list, (void *)k);
    CU_ASSERT(ret==CLIST_TRUE);
  }

  struct clist_iterator *it = malloc(sizeof(struct clist_iterator));
  ret = clist_get_iterator(&list, it, 0);

  ret = clist_iterate(&list, it, (void **)&k, 0);
  CU_ASSERT(ret==CLIST_TRUE);
  ctr = 0;
  do
  {
    printf("k->id: %i\n", k->id);
    ctr ++;
  } while (clist_iterate(&list, it, (void **)&k, 1)==CLIST_TRUE);
  CU_ASSERT(ctr==NUM_ELEMENTS*4);
  clist_destroy(&list);

}

void test_clist_sort(void)
{
  int items[] = {5, 10, 23, 12, 1,
                 91, 32, 12, 3, 2};
#define NUM_ELEMENTS 10
  struct clist list;
  int ret = clist_init(&list, match_function, free_destroy_function, CLIST_UNSORTED);
  struct key *k = NULL;
  int ctr = 0;
  for (; ctr<NUM_ELEMENTS; ctr++)
  {
    k = (struct key*)malloc(sizeof(struct key));
    k->id = items[ctr];
    printf("adding id: %i\n", k->id);
    ret = clist_append(&list, (void *)k);
    CU_ASSERT(ret==CLIST_TRUE);
  }

/*traverse list before sort*/
  struct clist_iterator *it = malloc(sizeof(struct clist_iterator));
  ret = clist_get_iterator(&list, it, 0);
  CU_ASSERT(ret==CLIST_TRUE);

  ret = clist_iterate(&list, it, (void **)&k, 0);
  CU_ASSERT(ret==CLIST_TRUE);
  ctr = 0;
  do
  {
    printf("k->id: %i\n", k->id);
    ctr ++;
  } while (clist_iterate(&list, it, (void **)&k, 1)==CLIST_TRUE);
  CU_ASSERT(ctr==NUM_ELEMENTS);
  
  clist_qsort(&list, ASCENDING);

/*traverse list after sort*/
  ret = clist_get_iterator(&list, it, 0);
  CU_ASSERT(ret==CLIST_TRUE);
  ret = clist_iterate(&list, it, (void **)&k, 0);
  CU_ASSERT(ret==CLIST_TRUE);
  ctr = 0;
  int prev = k->id;;
  do
  {
    printf("k->id: %i, prev: %i\n", k->id, prev);
    CU_ASSERT(prev<=k->id);
    prev = k->id;
    ctr ++;
  } while (clist_iterate(&list, it, (void **)&k, 1)==CLIST_TRUE);
  CU_ASSERT(ctr==NUM_ELEMENTS);
  free(it);
  
  clist_destroy(&list);
}

void test_clist_iterator(void)
{
#define NUM_ELEMENTS 10
  struct clist list;
  int ret = clist_init(&list, match_function, free_destroy_function, CLIST_UNSORTED);
  struct key *k = NULL;
  int ctr = NUM_ELEMENTS;
  for (; ctr>0; ctr--)
  {
    k = (struct key*)malloc(sizeof(struct key));
    k->id = ctr;
    printf("adding id: %i\n", k->id);
    ret = clist_append(&list, (void *)k);
    CU_ASSERT(ret==CLIST_TRUE);
  }

  struct clist_iterator *it = malloc(sizeof(struct clist_iterator));
  ret = clist_get_iterator(&list, it, 0);
  CU_ASSERT(ret==CLIST_TRUE);

  ret = clist_iterate(&list, it, (void **)&k, 0);
  CU_ASSERT(ret==CLIST_TRUE);
  ctr = 0;
  do
  {
    printf("k->id: %i\n", k->id);
    CU_ASSERT(NUM_ELEMENTS-ctr==k->id);
    ctr ++;
  } while (clist_iterate(&list, it, (void **)&k, 1)==CLIST_TRUE);
  CU_ASSERT(ctr==NUM_ELEMENTS);
  clist_destroy(&list);


/* corner case test (iterator for 1 element list) */

  struct clist list2;
  ret = clist_init(&list2, match_function, free_destroy_function, CLIST_UNSORTED);
  struct key *k2 = NULL;
  
  k2 = (struct key*)malloc(sizeof(struct key));
  k2->id = 100;
  printf("adding id: %i\n", k2->id);
  ret = clist_append(&list2, (void *)k2);
  CU_ASSERT(ret==CLIST_TRUE);

  struct clist_iterator *it2 = malloc(sizeof(struct clist_iterator));
  ret = clist_get_iterator(&list2, it2, 0);
  CU_ASSERT(ret==CLIST_TRUE);

  ret = clist_iterate(&list2, it2, (void **)&k2, 0);
  CU_ASSERT(ret==CLIST_TRUE);
  ctr = 0;
  do
  {
    printf("k2->id: %i\n", k2->id);
    ctr ++;
  } while (clist_iterate(&list2, it2, (void **)&k, 1)==CLIST_TRUE);
  CU_ASSERT(ctr==1);
  clist_destroy(&list2);

}

void test_clist_pop(void)
{
  struct key k1;
  struct key k2;
  struct key k3;
  k1.id = 1;
  k2.id = 2;
  k3.id = 3;
  struct clist list;
  int ret = clist_init(&list, match_function, empty_destroy_function, CLIST_UNSORTED);
  CU_ASSERT(ret == CLIST_TRUE);

  ret = clist_append(&list, (void *)&k1);
  CU_ASSERT(ret == CLIST_TRUE);
  ret = clist_append(&list, (void *)&k2);
  CU_ASSERT(ret == CLIST_TRUE);
  ret = clist_append(&list, (void *)&k3);
  CU_ASSERT(ret == CLIST_TRUE);
  struct key *data;

  /* struct clist_node * n = list.head; */
  
  ret = clist_get_by_id(&list, (void **)&data, -1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret == CLIST_TRUE)
    CU_ASSERT(data->id == 3);

  ret = clist_pop(&list);
  CU_ASSERT(ret == CLIST_TRUE);

  ret = clist_get_by_id(&list, (void **)&data, -1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret == CLIST_TRUE)
    CU_ASSERT(data->id == 2);

  clist_destroy(&list);  
}

void test_clist_insert(void)
{
  struct key k1;
  struct key k2;
  struct key k3;
  k1.id = 1;
  k2.id = 2;
  k3.id = 3;
  struct clist list;
  clist_init(&list, match_function, empty_destroy_function, CLIST_UNSORTED);
  int ret = clist_append(&list, (void *)&k1);

  CU_ASSERT(ret == CLIST_TRUE);
  ret = clist_append(&list, (void *)&k2);
  CU_ASSERT(ret == CLIST_TRUE);
  ret = clist_insert(&list, (void *)&k3, 1);
  CU_ASSERT(ret == CLIST_TRUE);

  struct key *data;
  ret = clist_get_by_id(&list, (void **)&data, 1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret == CLIST_TRUE)
    CU_ASSERT(data->id == 3);

  ret = clist_destroy(&list);
  CU_ASSERT(ret == CLIST_TRUE);
}

void test_clist_remove(void)
{
  struct key k1;
  struct key k2;
  struct key k3;
  k1.id = 1;
  k2.id = 2;
  k3.id = 3;
  struct clist list;
  clist_init(&list, match_function, empty_destroy_function, CLIST_UNSORTED);

  int ret = clist_append(&list, (void *)&k1);
  CU_ASSERT(ret == CLIST_TRUE);
  ret = clist_append(&list, (void *)&k2);
  CU_ASSERT(ret == CLIST_TRUE);
  ret = clist_append(&list, (void *)&k3);
  CU_ASSERT(ret == CLIST_TRUE);

  struct key *data;

  ret = clist_get_by_id(&list, (void **)&data, 1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret == CLIST_TRUE)
    CU_ASSERT(data->id == 2);

  ret = clist_remove(&list, (void **)&data, 1);
  CU_ASSERT(ret == CLIST_TRUE);

  ret = clist_get_by_id(&list, (void **)&data, 1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret == CLIST_TRUE)
    CU_ASSERT(data->id == 3);
  
  clist_destroy(&list);

  /* test corner case (one element list) */

  k1.id = 1;
  clist_init(&list, match_function, empty_destroy_function, CLIST_UNSORTED);

  ret = clist_append(&list, (void *)&k1);
  CU_ASSERT(ret == CLIST_TRUE);

  ret = clist_get_by_id(&list, (void **)&data, 0);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret == CLIST_TRUE)
    CU_ASSERT(data->id == 1);

  ret = clist_remove(&list, (void **)&data, 0);
  CU_ASSERT(ret == CLIST_TRUE);
  
  clist_destroy(&list);

}

void test_clist_append(void)
{
  struct key k1;
  struct key k2;
  k1.id = 1;
  k2.id = 2;
  struct clist list;
  clist_init(&list, match_function, empty_destroy_function, CLIST_UNSORTED);
  int ret = clist_append(&list, (void *)&k1);
  CU_ASSERT(ret == CLIST_TRUE);
  ret = clist_append(&list, (void *)&k2);
  CU_ASSERT(ret == CLIST_TRUE);
  struct key *data;
  ret = clist_get_by_id(&list, (void **)&data, -1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret == CLIST_TRUE)
    CU_ASSERT(data->id == 2);
  clist_destroy(&list);  
}

void test_clist_get_by_id(void)
{
  struct key k1;
  struct key k2;
  struct key k3;
  k1.id = 1;
  k2.id = 2;
  k3.id = 3;
  struct clist list;
  clist_init(&list, match_function, empty_destroy_function, CLIST_UNSORTED);
  clist_append(&list, (void *)&k1);
  clist_append(&list, (void *)&k2);
  clist_append(&list, (void *)&k3);

  struct key *data;

  int ret = clist_get_by_id(&list, (void **)&data, 1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret==CLIST_TRUE)
    CU_ASSERT(data->id == 2);

  ret = clist_get_by_id(&list, (void **)&data, 0);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret==CLIST_TRUE)
    CU_ASSERT(data->id == 1);

  clist_get_by_id(&list, (void **)&data, -1);
  CU_ASSERT(ret == CLIST_TRUE);
  if (ret==CLIST_TRUE)
    CU_ASSERT(data->id == 3);

  clist_destroy(&list);
}

int main()
{
  CU_pSuite pSuite = NULL;
  
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  
  pSuite = CU_add_suite("clist suite", init_clist_test_suite, clean_clist_test_suite);
  if (NULL == pSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_clist_pop))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_clist_insert))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_clist_remove))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_clist_append))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_clist_get_by_id))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_clist_iterator))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite,  test_clist_append_insert_append))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite,  test_clist_sort))
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
