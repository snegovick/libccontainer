#ifndef __CIRCBUF_H__
#define __CIRCBUF_H__

#include <stdint.h>
#include "clist_common.h"
#include <string.h>

struct circular_buffer {
  uint8_t *storage;
  unsigned int storage_size;
  unsigned int top;
  unsigned int bottom;
  unsigned int free_space;
};

int cb_init(struct circular_buffer *cb, uint8_t *storage, unsigned int storage_size) {
  cb->storage = storage;
  cb->storage_size = storage_size;
  cb->top = 0;
  cb->bottom = 0;
  cb->free_space = cb->storage_size;
  return CERR_OK;
}

unsigned int cb_get_free_space(struct circular_buffer *cb) {
  return cb->free_space;
}

unsigned int cb_get_allocated_space(struct circular_buffer *cb) {
  return (cb->storage_size-cb->free_space);
}

int cb_push(struct circular_buffer *cb, uint8_t *data, unsigned int size) {
  if (cb->free_space<size) {
    return CERR_ENOMEM;
  }
  unsigned int sz = (cb->storage_size - cb->bottom);
  if (sz < size) {
    memcpy(cb->storage+cb->bottom, data, sz);
    memcpy(cb->storage, data+sz, size-sz);
    cb->bottom = size-sz;
  } else {
    memcpy(cb->storage+cb->bottom, data, size);
    cb->bottom += size;
  }
  cb->free_space -= size;
  return size;
}

int cb_pop(struct circular_buffer *cb, uint8_t *data, unsigned int size) {
  if (cb->top==cb->bottom) {
    return CERR_NOENTRIES;
  }
  
  unsigned int allocated_size = cb_get_allocated_space(cb);
  if (allocated_size<size) {
    size = allocated_size;
  }

  unsigned int sz = cb->storage_size-cb->top;
  if (sz < size) {
    memcpy(data, cb->storage+cb->top, sz);
    memcpy(data+sz, cb->storage, size-sz);
    cb->top=size-sz;
  } else {
    memcpy(data, cb->storage+cb->top, size);
    cb->top+=size;
  }
  cb->free_space += size;
  return size;
}

#endif/*__CIRCBUF_H__*/
