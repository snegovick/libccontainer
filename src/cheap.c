#include "cheap.h"
#include "stdio.h"

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

int main(void) {
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
    
}
