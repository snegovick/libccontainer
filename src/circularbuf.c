#include "circularbuf.h"
#include <stdio.h>

int main(void) {
  char buf[] = {0xaa, 0xbb, 0xcc};
  char storage[256];

  struct circular_buffer cb;
  cb_init(&cb, storage, 256);

  while (1) {
    printf("allocating\r\n");
    if (cb_push(&cb, buf, 3)==CERR_ENOMEM) {
      printf("out of memory\r\n");
      break;
    }
    printf("free_space: %i\r\n", cb_get_free_space(&cb));
  }
}
