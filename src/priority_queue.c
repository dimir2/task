#include "priority_queue.h"

#include <stdio.h>

typedef struct Item Item;
typedef struct Elem Elem;
typedef struct Heap Heap;
typedef struct PrQueue PrQueue;

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
int elemCompare(Elem* left, Elem* right);

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

int elemCompare(Elem* left, Elem* right) {
  return (left->n_priority - right->n_priority);
}

int heapCreate(Heap** pp_heap, size_t size);
void heapHeapifyUp(Heap* p_heap, int n_count);
void heapHeapifyDown(Heap* p_heap, int n_parent);

Elem* heapFind(Heap* p_heap, int n_priority);

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
  for (int i = 0; i < p_heap->n_capacity; i++){
    elemDelete(&p_heap->pp_arr[i]);
  }
  free(p_heap);
}

void heapHeapifyUp(Heap* p_heap, int n_index) {}
void heapHeapifyDown(Heap* p_heap, int n_parent) {}

Elem* heapFind(Heap* p_heap, int n_priority) { return NULL; }
int heapPopMin(Heap* p_heap, int n_priority, void** data);


int heapInsert(Heap* p_heap, int n_priority, void* p_data) {
  Elem* p_elem = heapFind(p_heap, n_priority);
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
  heapHeapifyUp(p_heap, p_heap->n_count);
  p_heap->n_count++;
}

int heapPopMin(Heap* p_heap, int n_priority, void** data) {

}

struct PrQueue {
  Heap* p_heap;
  size_t n_count;
  size_t n_capacity;
  /* data */
};

int prQueueCreate(size_t n_capacity, PrQueue** pp_queue) {
  *pp_queue = (PrQueue*) malloc(sizeof(PrQueue));
  if(*pp_queue == NULL) {
    return STATUS_ERR_NOMEM;
  }
  (*pp_queue)->n_count = 0;
  (*pp_queue)->n_capacity = n_capacity;
  return heapCreate((*pp_queue)->p_heap, n_capacity);
}

int prQueuePush(PrQueue* p_queue, int n_priority, void* p_item) {
  return heapInsert(p_queue->p_heap, n_priority, p_item);
}

int prQueuePop(PrQueue* p_queue, int* pn_priority, void** pp_item);
int prQueuePeek(PrQueue* p_queue, int* priority, void** pp_item);
int prQueueGetCount(PrQueue * p_queue, size_t * pn_count);

void prQueueRelease(PrQueue * p_queue) {
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
  return 0;
}