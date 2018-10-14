#ifndef __LIST__
#define __LIST__

#include "types.h"

template <typename T>
struct Node {
    T* value;
    Node* next;
};

#endif