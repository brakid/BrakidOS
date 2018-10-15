#ifndef __LIST__
#define __LIST__

#include "types.h"

struct Node {
    void* value;
    Node* next;
};

struct List {
  Node* first;
  Node* last;
  int size;
};

Node* createNode();

List* createList();

void append(List* list, void* value);

bool remove(List* list, void* value);

int size(List* list);

void* get(List* list, int index);

int find(List* list, void* value);

#endif