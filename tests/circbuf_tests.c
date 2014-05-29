#include "CUnit/Basic.h"
#include <circularbuf.h>
#include <clist_common.h>
#include <stdint.h>
#include <stdio.h>

int init_circbuf_test_suite(void)
{
  return 0;
}

int clean_circbuf_test_suite(void)
{
  return 0;  
}

void test_alloc_till_enomem(void)
{
  char buf[] = {0xaa, 0xbb, 0xcc};
  char storage[256];

  struct circular_buffer cb;
  cb_init(&cb, storage, 256);

  int i = 0;
  while (1) {
    if (cb_push(&cb, buf, 3)==CERR_ENOMEM) {
      break;
    }
    i+=1;
  }
  CU_ASSERT(i==85);

  i = 0;
  while (1) {
    if (cb_pop(&cb, buf, 3)==CERR_NOENTRIES) {
      break;
    }
    i+=1;
  }
  CU_ASSERT(i==85);
}

int main()
{
  CU_pSuite pSuite = NULL;
  
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  
  pSuite = CU_add_suite("circular buffer test suite", init_circbuf_test_suite, clean_circbuf_test_suite);
  if (NULL == pSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_alloc_till_enomem))
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
