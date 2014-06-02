#ifndef __CIRCBUF_H__
#define __CIRCBUF_H__

#include <stdint.h>
#include "clist_common.h"

struct circular_buffer {
  uint8_t *storage;
  unsigned int storage_size;
  unsigned int top;
  unsigned int bottom;
  unsigned int free_space;
};

int cb_init(struct circular_buffer *cb, uint8_t *storage, unsigned int storage_size);
unsigned int cb_get_free_space(struct circular_buffer *cb);
unsigned int cb_get_allocated_space(struct circular_buffer *cb);
int cb_push(struct circular_buffer *cb, const uint8_t *data, unsigned int size);
int cb_push_byte(struct circular_buffer *cb, uint8_t b);
int cb_pop(struct circular_buffer *cb, uint8_t *data, unsigned int size);
unsigned int cb_get_current_position(struct circular_buffer *cb);
int cb_get_data(struct circular_buffer *cb, unsigned int start, unsigned int end, uint8_t *data, unsigned int data_buffer_size, unsigned int *acquired_size);
int cb_recover(struct circular_buffer *cb);

#endif/*__CIRCBUF_H__*/
