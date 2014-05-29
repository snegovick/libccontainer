#include "CUnit/Basic.h"
#include <cdynstorage.h>
#include <stdint.h>
#include <stdio.h>

void parr(char *buf, int sz) {
  char *p = buf;
  int i = 0;
  for (i; i<sz; i++) {
    printf("0x%02x ", (uint8_t)*p);
    if ((i+1)%8 == 0) {
      printf("\r\n");
    }
    p++;
  }
}

int init_cds_test_suite(void)
{
  return 0;
}

int clean_cds_test_suite(void)
{
  return 0;  
}

void test_alloc_till_null(void)
{
#define NUM_ELEMENTS 1024
  char buf[256];
  memset(buf, 0, 256);
  struct cdynstorage cds;
  printf ("\r\ninit\r\n");
  cds_init(&cds, buf, 256);
  parr(buf, 256);

  char *data[1024];

  int i = 0;
  for ( ; i<1024; i++ ) {
    printf ("\r\nalloc\r\n");
    data[i] = cds_alloc(&cds, 3);
    if (data[i] == NULL) {
      break;
    }
    data[i][0] = 0xaa;
    data[i][1] = 0xbb;
    data[i][2] = 0xcc;
    parr(buf, 256);
  }

  printf("\r\nfree\r\n");
  cds_free(&cds, data[0]);
  parr(buf, 256);
  printf("cds avail: %i, cds used: %i, cds n_ptrs: %i\r\n", cds.avail, cds.used, cds.n_ptrs_allocated);

  printf ("\r\nalloc\r\n");
  data[0] = cds_alloc(&cds, 3);
  data[0][0] = 0xae;
  data[0][1] = 0xea;
  data[0][2] = 0xdd;
  parr(buf, 256);

  CU_ASSERT(cds.n_ptrs_allocated==16);
}

int main()
{
  CU_pSuite pSuite = NULL;
  
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  
  pSuite = CU_add_suite("cdynstorage test suite", init_cds_test_suite, clean_cds_test_suite);
  if (NULL == pSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_ADD_TEST(pSuite, test_alloc_till_null))
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
