#include "priority_queue.h"

typedef struct ListItem ListItem;

struct ListItem {
  void** pp_data;
  ListItem* p_next;
};

int listItemCreate(ListItem** pp_list_item, void** pp_data) {
  *pp_list_item = (ListItem*)malloc(sizeof(ListItem));
  if (*pp_list_item == NULL) {
    return STATUS_ERR_NOMEM;
  }
  (*pp_list_item)->pp_data = pp_data;
  (*pp_list_item)->p_next = NULL;
  return STATUS_OK;
}

int listItemDelete(ListItem** pp_list_item) { free(*pp_list_item); }

typedef struct _Item {
  int n_priority;
  ListItem* p_next;
  ListItem* p_last
} Item;

int itemCreate(Item** pp_item, int n_priority, void* p_data) {
  Item* p_item = malloc(sizeof(Item));
  if (p_item == NULL) {
    return STATUS_ERR_NOMEM;
  }
  *pp_item = (Item*)p_item;
  return STATUS_OK;
}

void itemDelete(Item** pp_item) {
  if (*pp_item != NULL) {
    free(*pp_item);
  }
}

typedef struct _Heap {
} Heap;

typedef struct _PrQueue {
  Heap* ph;
  size_t n_count;
  size_t n_capacity;
  /* data */
} PrQueue;

int main() {
  ListItem* p_item;
  void* p_data;
  int ret = listItemCreate(&p_item, &p_data);
  if (ret == STATUS_OK) {
    printf("OK");
  }
  return 0;
}