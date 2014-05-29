#ifndef __CDS_H__
#define __CDS_H__

#include "clist_common.h"
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif


struct cdynstorage {
  uint8_t *storage;
  unsigned int storage_size;
  unsigned int avail;
  unsigned int used;
  unsigned int n_ptrs_allocated;
};

struct cds_block_header {
  unsigned int size;
  unsigned int next_ptr;
  unsigned int prev_ptr;
};

#define ATTACH_THRESHOLD (2*H_SZ)

int cds_init(struct cdynstorage *cds, uint8_t *storage, unsigned int storage_size);
void *cds_alloc(struct cdynstorage *cds, unsigned int size);
void cds_free(struct cdynstorage *cds, void *ptr);

#endif/*__CDS_H__*/
