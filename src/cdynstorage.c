#include "cdynstorage.h"
#include "stdio.h"

#define get_bh(i) ((struct cds_block_header *)&cds->storage[i])

#define UINT_MAX ((unsigned int)(-1))
#define H_SZ sizeof(struct cds_block_header)

int cds_init(struct cdynstorage *cds, uint8_t *storage, unsigned int storage_size) {
  cds->storage = storage;
  cds->storage_size = storage_size;
  struct cds_block_header *cabh = (struct cds_block_header *)cds->storage;
  cabh->size = storage_size;
  cabh->next_ptr = UINT_MAX;
  cabh->prev_ptr = UINT_MAX;
  cds->avail = 0;
  cds->used = 0;
  cds->n_ptrs_allocated = 0;
  return CERR_OK;
}

int __cds_find_fitting_free(struct cdynstorage *cds, unsigned int size, struct cds_block_header **cbh, unsigned int *offset) {
  if (size == 0) {
    return CERR_GENERAL_ERROR;
  } else if (size > cds->storage_size) {
    return CERR_ENOMEM;
  }
  
  unsigned int p = cds->avail;
  struct cds_block_header *bh;

  while (1) {
    bh = (struct cds_block_header *)&cds->storage[p];
    if (bh->size >= size) {
      *offset = p;
      *cbh = bh;
      return CERR_OK;
    } else if (bh->next_ptr == UINT_MAX) {
      return CERR_ENOMEM;
    } else {
      p = bh->next_ptr;
    }
  }
  return CERR_GENERAL_ERROR;
}

int __cds_find_prev_free(struct cdynstorage *cds, unsigned int offset, struct cds_block_header **cbh, unsigned int *block_offset) {
  unsigned int p = cds->avail;
  struct cds_block_header *bh;

  while (1) {
    bh = get_bh(p);
    if (p > offset) {
      p = bh->prev_ptr;
      if (p == UINT_MAX) {
        return CERR_ENOTFOUND;
      }
      bh = get_bh(p);
      *block_offset = p;
      *cbh = bh;
      return CERR_OK;
    } else if (bh->next_ptr == UINT_MAX) {
      *block_offset = p;
      *cbh = bh;
      return CERR_OK;
    } else {
      p = bh->next_ptr;
    }
  }
  return CERR_GENERAL_ERROR;
}


int __cds_find_prev_used(struct cdynstorage *cds, unsigned int offset, struct cds_block_header **cbh, unsigned int *block_offset) {
  unsigned int p = cds->used;
  struct cds_block_header *bh;

  while (1) {
    bh = get_bh(p);
    if (p > offset) {
      p = bh->prev_ptr;
      if (p == UINT_MAX) {
        return CERR_ENOTFOUND;
      }
      bh = get_bh(p);
      *block_offset = p;
      *cbh = bh;
      return CERR_OK;
    } else if (bh->next_ptr == UINT_MAX) {
      *block_offset = p;
      *cbh = bh;
      return CERR_OK;
    } else {
      p = bh->next_ptr;
    }
  }
  return CERR_GENERAL_ERROR;
}

int __cds_find_matching_used(struct cdynstorage *cds, unsigned int offset, struct cds_block_header **cbh, unsigned int *block_offset) {
  unsigned int p = cds->used;
  struct cds_block_header *bh;
  
  while (1) {
    bh = get_bh(p);
    if ((p + bh->size) >= offset) {
      *cbh = bh;
      *block_offset = p;
      return CERR_OK;
    } else if (bh->next_ptr == UINT_MAX) {
      return CERR_ENOTFOUND;
    } else {
      p = bh->next_ptr;
      bh = get_bh(p);
    }
  }
  return CERR_GENERAL_ERROR;
}

void __swap_ptrs(struct cdynstorage *cds, unsigned int next_id, unsigned int prev_id, unsigned int prev_next, unsigned int next_prev) {
  if (next_id != UINT_MAX) {
    struct cds_block_header *nh = get_bh(next_id);
    nh->prev_ptr = next_prev;
  }
  if (prev_id != UINT_MAX) {
    struct cds_block_header *ph = get_bh(prev_id);
    ph->next_ptr = prev_next;
  }
}

void *cds_alloc(struct cdynstorage *cds, unsigned int size) {
  if (cds->avail==UINT_MAX) {
    return NULL;
  }

  struct cds_block_header *cbh;
  unsigned int p = 0;
  size = size + H_SZ;
  int ret = __cds_find_fitting_free(cds, size, &cbh, &p);
  if (ret!=CERR_OK) {
    return NULL;
  }
  
  unsigned int q = 0;
  unsigned int old_size = cbh->size;
  if ((cbh->size - size) <= ATTACH_THRESHOLD) {
    size = cbh->size;
    __swap_ptrs(cds, cbh->next_ptr, cbh->prev_ptr, cbh->next_ptr, cbh->prev_ptr);
    q = cbh->next_ptr;
  } else {
    q = p + size;
    struct cds_block_header *bh = (struct cds_block_header *)&cds->storage[q];
    bh->next_ptr = cbh->next_ptr;
    bh->prev_ptr = cbh->prev_ptr;
    __swap_ptrs(cds, bh->next_ptr, bh->prev_ptr, q, q);
    bh->size = old_size-size;
  }
  
  if (p == cds->avail) {
    cds->avail = q;
  }

  struct cds_block_header *used_cbh = (struct cds_block_header *)&cds->storage[p];
  if (cds->n_ptrs_allocated == 0) {
    used_cbh->prev_ptr = UINT_MAX;    
    used_cbh->next_ptr = UINT_MAX;
    cds->used = p;
  } else {
    struct cds_block_header *used_prev;
    unsigned int block_offset;
    ret = __cds_find_prev_used(cds, p, &used_prev, &block_offset);
    if (ret == CERR_ENOTFOUND) {
      block_offset = cds->used;
      struct cds_block_header *first_used = get_bh(block_offset);
      first_used->prev_ptr = p;
      used_cbh->next_ptr = block_offset;
      used_cbh->prev_ptr = UINT_MAX;
    }
    else if (ret != CERR_OK) {
      return NULL;
    } else {
      used_cbh->prev_ptr = block_offset;
      if (used_prev->next_ptr != UINT_MAX) {
        struct cds_block_header *used_next = get_bh(used_prev->next_ptr);
        used_next->prev_ptr = p;
      }
      used_cbh->next_ptr = used_prev->next_ptr;
      used_prev->next_ptr = p;
    }
  }
  used_cbh->size = size;

  
  cds->n_ptrs_allocated++;

  return (void*)&cds->storage[p+H_SZ];
}

void cds_free(struct cdynstorage *cds, void *ptr) {
  if (cds->storage>(uint8_t*)ptr) {
    return;
  } else if (cds->storage+cds->storage_size<(uint8_t*)ptr) {
    return;
  }
  unsigned int offset = ((uint8_t*)ptr - cds->storage);
  struct cds_block_header *bh = NULL;
  unsigned int p = 0;
  int ret = __cds_find_matching_used(cds, offset, &bh, &p);
  unsigned int size = bh->size;
  if (ret!=CERR_OK) {
    return;
  } else {
    if (bh->prev_ptr == UINT_MAX) {
      cds->used = bh->next_ptr;
    }
    __swap_ptrs(cds, bh->next_ptr, bh->prev_ptr, bh->next_ptr, bh->prev_ptr);
  }

  offset = p;
  if (cds->avail == UINT_MAX) {
    bh = (struct cds_block_header *)&cds->storage[offset];
    bh->size = size;
    bh->next_ptr = UINT_MAX;
    bh->prev_ptr = UINT_MAX;
    cds->avail = offset;
    cds->n_ptrs_allocated --;
    return;
  }
  ret = __cds_find_prev_free(cds, offset, &bh, &p);
  //assume that free memory is somewhere ahead
  if (ret == CERR_ENOTFOUND) {
    struct cds_block_header *next = get_bh(cds->avail);
    bh = (struct cds_block_header *)&cds->storage[offset];
    if (offset+size==cds->avail) {
      bh->next_ptr = next->next_ptr;
      bh->prev_ptr = UINT_MAX;
      if (bh->next_ptr != UINT_MAX) {
        struct cds_block_header *nextnext = get_bh(bh->next_ptr);
        nextnext->prev_ptr = offset;
      }
      bh->size = size + next->size;
    } else {
      bh->size = size;
      bh->next_ptr = cds->avail;
      bh->prev_ptr = UINT_MAX;
      next->prev_ptr = offset;
    }
    cds->avail = offset;
    cds->n_ptrs_allocated --;
    return;
  } else if (ret != CERR_OK) {
    return;
  }
  if ((p+bh->size) == offset) {
    bh->size = bh->size + size;
    if (bh->next_ptr != UINT_MAX) {
      if (bh->next_ptr == (p+bh->size)) {
        struct cds_block_header *next = get_bh(bh->next_ptr);
        bh->next_ptr = next->next_ptr;
        bh->size = bh->size + next->size;
      }
    }
  } else if ((offset+size) == bh->next_ptr) {
    struct cds_block_header *cbh = get_bh(offset);
    struct cds_block_header *next = get_bh(bh->next_ptr);
    cbh->next_ptr = next->next_ptr;
    cbh->prev_ptr = next->prev_ptr;
    __swap_ptrs(cds, cbh->next_ptr, cbh->prev_ptr, offset, offset);
    cbh->size = size + next->size;
  } else {
    struct cds_block_header *cbh = get_bh(offset);
    cbh->next_ptr = bh->next_ptr;
    cbh->prev_ptr = p;
    __swap_ptrs(cds, cbh->next_ptr, cbh->prev_ptr, offset, offset);
    cbh->size = size;
  }
  cds->n_ptrs_allocated --;
}
