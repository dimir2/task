#include "priority_queue.h"

#include <stdio.h>

typedef struct Item Item;
typedef struct Elem Elem;
typedef struct Heap Heap;

int itemCreate(Item** pp_item, void* p_data);
void itemDelete(Item* p_item);

/*  This structure is a part of the list structure aiming to store the
    poiter to the data for the heap elements with the same priority.
*/
struct Item {
  void* p_data;
  Item* p_next;
};

int itemCreate(Item** pp_item, void* p_data) {
  *pp_item = (Item*)malloc(sizeof(Item));
  if (*pp_item == NULL) {
    return STATUS_ERR_NOMEM;
  }
  (*pp_item)->p_data = p_data;
  (*pp_item)->p_next = NULL;
  return STATUS_OK;
}

void itemDelete(Item* p_item) { free(p_item); }

int elemCreate(Elem** pp_elem, int n_priority, void* p_data);
int elemHasData(Elem* p_elem);
int elemAddData(Elem* p_elem, void* p_data);
int elemGetData(Elem* p_elem, void** pp_data);
int elemDeleteData(Elem* p_elem);
void elemDelete(Elem* p_elem);

/*  This structure is a part of heap is a holder for the priority
    and all data, assosiated with this priority.
    Data items are stored as a list with the oldest element at the head.
    New Data item are placed into the tail of the list.
*/
struct Elem {
  int n_priority;
  Item* p_head;
  Item* p_tail;
};

int elemHasData(Elem* p_elem) { return (p_elem->p_head != NULL); }

int elemCreate(Elem** pp_elem, int n_priority, void* p_data) {
  *pp_elem = (Elem*)malloc(sizeof(Elem));
  if (*pp_elem == NULL) {
    return STATUS_ERR_NOMEM;
  }
  (*pp_elem)->n_priority = n_priority;
  int ret = itemCreate(&(*pp_elem)->p_head, p_data);
  if (ret != STATUS_OK) {
    free(*pp_elem);
    *pp_elem = NULL;
    return STATUS_ERR_NOMEM;
  }
  (*pp_elem)->p_tail = (*pp_elem)->p_head;
  return STATUS_OK;
}

int elemAddData(Elem* p_elem, void* p_data) {
  int ret = itemCreate(&p_elem->p_tail->p_next, p_data);
  if (ret != STATUS_OK) {
    return STATUS_ERR_NOMEM;
  }
  p_elem->p_tail = p_elem->p_tail->p_next;
  return STATUS_OK;
}

int elemGetData(Elem* p_elem, void** pp_data) {
  if (p_elem->p_head == NULL) {
    return STATUS_ERR_NODATA;
  }
  *pp_data = p_elem->p_head->p_data;
  return STATUS_OK;
}

int elemDeleteData(Elem* p_elem) {
  if (elemHasData(p_elem)) {
    Item* p_item = p_elem->p_head;
    p_elem->p_head = p_elem->p_head->p_next;
    if (p_elem->p_head == NULL) {
      p_elem->p_tail = NULL;
    }
    itemDelete(p_item);
  }
  return STATUS_OK;
}

void elemDelete(Elem* p_elem) {
  while (elemHasData(p_elem)) {
    elemDeleteData(p_elem);
  }
  free(p_elem);
}

int heapCreate(Heap** pp_heap, size_t size);
void heapDelete(Heap* p_heap);
int heapPop(Heap* p_heap, int* n_priority, void** pp_data);
int heapPeek(Heap* p_heap, int* n_priority, void** pp_data);

void _hHeapifyUp(Heap* p_heap, int n_count);
void _hHeapifyDown(Heap* p_heap, int n_parent);
Elem* _hSearch(Heap* p_heap, int n_priority);
void _hSwapElems(Heap* p_heap, int n_left, int n_right);
int _hCompareElems(Heap* p_heap, int n_left, int n_right);

struct Heap {
  Elem** pp_arr;
  size_t n_capacity;
  size_t n_count;
};

int heapCreate(Heap** pp_heap, size_t size) {
  *pp_heap = (Heap*)malloc(sizeof(Heap));
  if (*pp_heap == NULL) {
    return STATUS_ERR_NOMEM;
  }
  (*pp_heap)->pp_arr = (Elem**)calloc(size, sizeof(Elem*));
  if ((*pp_heap)->pp_arr == NULL) {
    free(*pp_heap);
    *pp_heap = NULL;
    return STATUS_ERR_NOMEM;
  }
  (*pp_heap)->n_capacity = size;
  (*pp_heap)->n_count = 0;
  return STATUS_OK;
}

void heapDelete(Heap* p_heap) {
  for (int i = 0; i < p_heap->n_capacity; i++) {
    elemDelete(p_heap->pp_arr[i]);
  }
  free(p_heap);
}

void _hHeapifyUp(Heap* p_heap, int n_index) {
  int n_parent = (n_index - 1) / 2;
  if (p_heap->pp_arr[n_parent]->n_priority >
      p_heap->pp_arr[n_index]->n_priority) {
    _hSwapElems(p_heap, n_parent, n_index);
    _hHeapifyUp(p_heap, n_parent);
  }
}

void _hHeapifyDown(Heap* p_heap, int n_parent) {
  int n_left = n_parent * 2 + 1;
  int n_right = n_parent * 2 + 2;
  int n_minimum;
  if (n_left >= p_heap->n_count || n_left < 0) {
    n_left = -1;
  }
  if (n_right >= p_heap->n_count || n_right < 0) {
    n_right = -1;
  }
  if (n_left != -1 && _hCompareElems(p_heap, n_left, n_parent) < 0) {
    n_minimum = n_left;
  } else {
    n_minimum = n_parent;
  }
  if (n_right != -1 && _hCompareElems(p_heap, n_right, n_minimum) < 0) {
    n_minimum = n_right;
  }
  if (n_minimum != n_parent) {
    _hSwapElems(p_heap, n_minimum, n_parent);
    _hHeapifyDown(p_heap, n_minimum);
  }
}

void _hSwapElems(Heap* p_heap, int n_left, int n_right) {
  Elem* temp = p_heap->pp_arr[n_left];
  p_heap->pp_arr[n_left] = p_heap->pp_arr[n_right];
  p_heap->pp_arr[n_right] = temp;
}

Elem* _hSearch(Heap* p_heap, int n_priority) {
  for (int i = 0; i < p_heap->n_count; i++) {
    if (p_heap->pp_arr[i]->n_priority == n_priority) {
      return p_heap->pp_arr[i];
    }
  }
  return NULL;
}

int _hCompareElems(Heap* p_h, int n_l, int n_r) {
  return p_h->pp_arr[n_l]->n_priority - p_h->pp_arr[n_r]->n_priority;
}

int heapPeek(Heap* p_heap, int* pn_priority, void** pp_data) {
  if (p_heap->n_count == 0) {
    return STATUS_ERR_NODATA;
  }
  *pn_priority = p_heap->pp_arr[0]->n_priority;
  *pp_data = p_heap->pp_arr[0]->p_head->p_data;
  return STATUS_OK;
}

int heapPop(Heap* p_heap, int* pn_priority, void** pp_data) {
  if (STATUS_OK != heapPeek(p_heap, pn_priority, pp_data)) {
    return STATUS_ERR_NODATA;
  }
  elemDeleteData(p_heap->pp_arr[0]);
  if (!elemHasData(p_heap->pp_arr[0])) {
    elemDelete(p_heap->pp_arr[0]);
    _hSwapElems(p_heap, 0, p_heap->n_count - 1);
    p_heap->n_count--;
    _hHeapifyDown(p_heap, 0);
  }
}

int heapPush(Heap* p_heap, int n_priority, void* p_data) {
  Elem* p_elem = _hSearch(p_heap, n_priority);
  if (p_elem != NULL) {
    return elemAddData(p_elem, p_data);
  }
  if (p_heap->n_count == p_heap->n_capacity) {
    return STATUS_ERR_CAPACITY;
  }
  if (STATUS_OK !=
      elemCreate(&p_heap->pp_arr[p_heap->n_count], n_priority, p_data)) {
    return STATUS_ERR_NOMEM;
  }
  _hHeapifyUp(p_heap, p_heap->n_count);
  p_heap->n_count++;
  return STATUS_OK;
}

struct _PrQueue {
  Heap* p_heap;
};

int prQueueCreate(size_t n_capacity, PrQueue** pp_queue) {
  if (pp_queue == NULL) {
    return STATUS_ERR_INVALID;
  }
  *pp_queue = (PrQueue*)malloc(sizeof(PrQueue));
  if (*pp_queue == NULL) {
    return STATUS_ERR_NOMEM;
  }
  return heapCreate(&(*pp_queue)->p_heap, n_capacity);
}

int prQueuePush(PrQueue* p_queue, int n_priority, void* p_item) {
  if (p_queue == NULL) {
    return STATUS_ERR_INVALID;
  }
  return heapPush(p_queue->p_heap, n_priority, p_item);
}

int prQueuePop(PrQueue* p_queue, int* pn_priority, void** pp_item) {
  if (p_queue == NULL || pn_priority == NULL || pp_item == NULL) {
    return STATUS_ERR_INVALID;
  }
  return heapPop(p_queue->p_heap, pn_priority, pp_item);
}

int prQueuePeek(PrQueue* p_queue, int* pn_priority, void** pp_item) {
  if (p_queue == NULL || pn_priority == NULL || pp_item == NULL) {
    return STATUS_ERR_INVALID;
  }
  return heapPeek(p_queue->p_heap, pn_priority, pp_item);
}

int prQueueGetCount(PrQueue* p_queue, size_t* pn_count) {
  if (p_queue == NULL || pn_count == NULL) {
    return STATUS_ERR_INVALID;
  }
  *pn_count = p_queue->p_heap->n_count;
  return STATUS_OK;
}

void prQueueRelease(PrQueue* p_queue) {
  heapDelete(p_queue->p_heap);
  free(p_queue);
}

int main() {
  Item* p_item;
  void* p_data;
  int ret = itemCreate(&p_item, &p_data);
  if (ret == STATUS_OK) {
    printf("OK");
  }
  Elem* p_elem;
  ret = elemCreate(&p_elem, 10, p_data);
  if (ret == STATUS_OK) {
    printf("OK");
  }

  Heap* p_heap;
  ret = heapCreate(&p_heap, 10);
  if (ret == STATUS_OK) {
    printf("OK");
  }

  PrQueue * p_pr_queue;
  ret = prQueueCreate(10, &p_pr_queue);
  if (ret == STATUS_OK) {
    printf("OK\n");
  }

  ret = prQueuePush(p_pr_queue, 3, p_data);
  ret = prQueuePush(p_pr_queue, 5, p_data);
  ret = prQueuePush(p_pr_queue, 1, p_data);
  ret = prQueuePush(p_pr_queue, 5, p_data);
  ret = prQueuePush(p_pr_queue, 10, p_data);
  
  int n_priority;
  ret = prQueuePop(p_pr_queue, &n_priority, &p_data);
  printf("priority = %d\n", n_priority);
  size_t st_queue_count;
  ret = prQueueGetCount(p_pr_queue, &st_queue_count);
  printf("count = %ld\n", st_queue_count);


  return 0;
}