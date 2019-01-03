#include <clist.h>

int clist_init(struct clist *list, int (*match)(const void *key1, const void* key2), void (*destroy)(void *data), unsigned int sorted)
{
  list->sorted = sorted;
  list->destroy = destroy;
  list->match = match;
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
  return CLIST_TRUE;
}

int __clist_get_node_by_id(struct clist *list, struct clist_node **node, int id)
{
  if ((abs(id)>list->size) || list->size == 0 || list->head == NULL)
  {
    *node = NULL;
    return CLIST_ENOTFOUND;
  }
  if (id == -1)
  {
    *node = list->tail;
    return CLIST_TRUE;
  }
  if (id == 0)
  {
    *node = list->head;
    return CLIST_TRUE;
  }
  
  int i;
  if (id > 0) {
    struct clist_node * t = list->head;
    for (i = 0; i < id; i++)
    {
      if (t->next == NULL)
      {
        *node = NULL;
        return CLIST_ENOTFOUND;
      }
      t = t->next;
    }
    *node = t;
  } else {
    struct clist_node * t = list->tail;
    for (i = 0; i < abs(id); i++)
    {
      if (t->prev == NULL)
      {
        *node = NULL;
        return CLIST_ENOTFOUND;
      }
      t = t->prev;
    }
    *node = t;
    
  }
  return CLIST_TRUE;
}


int clist_get_by_id(struct clist *list, void **data, int id)
{
  struct clist_node * node;
  *data = NULL;

  
  int ret = __clist_get_node_by_id(list, &node, id);
  if (ret == CLIST_TRUE)
  {
    if (node != NULL)
      *data = node->data;
  }
  return ret;
}

int clist_destroy(struct clist *list)
{
  (void)list;
  return CLIST_TRUE;
}

int __clist_start_list(struct clist *list, void *data)
{
  if (list->size==0)
  {
    void *td = clist_malloc(sizeof(struct clist_node));
    if (td==NULL)
      return CLIST_FALSE;
    list->head = (struct clist_node*)td;
    list->head->next = (struct clist_node*)NULL;
    list->head->prev = (struct clist_node*)NULL;
    list->head->data = data;
    list->size = 1;
    list->tail = list->head;
    return CLIST_TRUE;
  }
  return CLIST_ENOTEMPTY;
}

int clist_append(struct clist *list, void *data)
{
  if (__clist_start_list(list, data) == CLIST_TRUE)
    return CLIST_TRUE;

  void* td = clist_malloc(sizeof(struct clist_node));
  if (td==NULL)
    return CLIST_FALSE;
  struct clist_node *n = (struct clist_node*)td;
  list->tail->next = n;
  n->next = (struct clist_node*)NULL;
  n->prev = list->tail;
  n->data = data;
  list->size ++;
  list->tail = n;
  
  return CLIST_TRUE;
}

int clist_pop(struct clist *list)
{
  if (list->tail == NULL || list->size == 0)
    return CLIST_FALSE;
  struct clist_node * prev = list->tail->prev;
  void *data = list->tail->data;
  list->destroy(data);
  clist_free(list->tail);
  list->tail = prev;
  list->size--;
  return CLIST_TRUE;
}

int clist_remove(struct clist *list, void **data, int id)
{
  struct clist_node * node;
  int ret = __clist_get_node_by_id(list, &node, id);
  if (ret != CLIST_TRUE)
  {
    return CLIST_FALSE;
  }
  *data = NULL;
  struct clist_node *prev = node->prev;
  struct clist_node *next = node->next;
  if (prev!=NULL) {
    prev->next = next;
  } else {
    list->head = next;
  }
  if (next!=NULL) {
    next->prev = prev;
  } else {
    list->tail = prev;
  }


  list->destroy(node->data);
  node->data = NULL;
  clist_free(node);
  list->size--;
  return CLIST_TRUE;
}

int clist_insert(struct clist *list, void *data, int id)
{
  if (__clist_start_list(list, data) == CLIST_TRUE)
    return CLIST_TRUE;

  if (id==(int)list->size)
    return clist_append(list, data);

  struct clist_node *node;
  int ret = __clist_get_node_by_id(list, &node, id);
  if (ret!=CLIST_TRUE)
  {
    return CLIST_FALSE;
  }
  /* struct clist_node *prev = node->prev; */
  /* struct clist_node *next = node->next; */

  void* td = clist_malloc(sizeof(struct clist_node));
  if (td==NULL)
    return CLIST_FALSE;
  struct clist_node *n = (struct clist_node*)td;
  if (node->prev!=NULL)
    node->prev->next = n;
  n->prev = node->prev;
  n->next = node;
  n->data = data;
  node->prev = n;
  list->size ++;
  if (id==0)
    list->head = n;
  else if (id==-1)
    list->tail = n;
  return CLIST_TRUE;
}

int clist_get_iterator(struct clist *list, struct clist_iterator *it, int start)
{
  struct clist_node *n = NULL;
  it->n = NULL;
  int ret = __clist_get_node_by_id(list, &n, start);
  if (ret == CLIST_TRUE)
  {
    if (n != NULL)
    {
      it->n = n;
      it->id = start;
    }
    else
      ret = CLIST_FALSE;
  }
  return ret;
}

int clist_iterate(struct clist *list, struct clist_iterator *it, void **data, int step)
{
  (void)list;
  if (it->n == NULL)
    return CLIST_FALSE;

  int i = 0;
  struct clist_node *n = it->n;
  if (step>0)
  {
    for (i=0; i<step; i++)
    {
      if (n->next==NULL)
        return CLIST_FALSE;
      n = n->next;
    }
  }
  else if (step<0)
  {
    for (i=0; i>step; i--)
    {
      if (n->prev==NULL)
        return CLIST_FALSE;
      n = n->prev;
    }
  }
  it->n = n;
  it->id+=step;
  if ((it->n != NULL) && (data != NULL))
    *data = it->n->data;
  else if (it->n == NULL)
    return CLIST_FALSE;
  return CLIST_TRUE;
}

int clist_post_iterate(struct clist *list, struct clist_iterator *it, void **data, int step) {
  (void)list;
  if (it->n == NULL)
    return CLIST_FALSE;


  if ((it->n != NULL) && (data != NULL)) {
    *data = it->n->data;
  } else if (it->n == NULL) {
    return CLIST_FALSE;
  }

  int i = 0;
  struct clist_node *n = it->n;
  if (step>0)
  {
    for (i=0; i<step; i++)
    {
      if (n->next==NULL)
        return CLIST_FALSE;
      n = n->next;
    }
  }
  else if (step<0)
  {
    for (i=0; i>step; i--)
    {
      if (n->prev==NULL)
        return CLIST_FALSE;
      n = n->prev;
    }
  }
  it->n = n;
  it->id+=step;

  return CLIST_TRUE;
}

void __swap_data(struct clist_node *n1, struct clist_node *n2) {
  void *data = n1->data;
  n1->data = n2->data;
  n2->data = data;
}

void __qs_subproc(struct clist *list, int first, int last) {
  if (first>=last) {
    return;
  }
  int x_pos = (first+last)/2;
  struct clist_node *x;
  struct clist_node *f;
  struct clist_node *f_node;
  struct clist_node *l;
  int i = first;
  int j = last;
  if (__clist_get_node_by_id(list, &f, first) != CLIST_TRUE) {
    return;
  }
  if (__clist_get_node_by_id(list, &l, last) != CLIST_TRUE) {
    return;
  }
  if (__clist_get_node_by_id(list, &x, x_pos) != CLIST_TRUE) {
    return;
  }

  f_node = f;
  __swap_data(f, x);
  x = f;
  f = f->next;
  i++;
  
  while (i <= j) {
    while (list->match(l->data, x->data) == 1) {
      j --;
      l = l->prev;
    }

    while ((i<=j) && (list->match(f->data, x->data) == -1)) {
      i ++;
      f = f->next;
    }

    if (i <= j) {
      __swap_data(f, l);
      i ++;
      j --;
      f = f->next;
      l = l->prev;
    }
  }
  __swap_data(f_node, l);  
  __qs_subproc(list, first, j-1);
  __qs_subproc(list, j+1, last);
}

int clist_qsort(struct clist *list, enum sort_order order) {
  (void)order;
  __qs_subproc(list, 0, list->size - 1);
  return CLIST_TRUE;
}
