#include "list.h"
#include "io.h"
#include "constants.h"
#include "utils.h"
#include "memory.h"

Node* createNode() {
    enterCritical();
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = 0;
    node->next = 0;
    leaveCritical();
    return node;
}

List* createList() {
    enterCritical();
    List* list = (List*)malloc(sizeof(List));
    list->first = 0;
    list->last = 0;
    list->size = 0;
    leaveCritical(); 
    return list;  
}

void append(List* list, void* value) {
    enterCritical();
    Node* node = createNode();
    node->value = value;

    if (list->size == 0) {
        list->first = node;
    } else {
        list->last->next = node;
    }
    list->last = node;
    list->size++;

    leaveCritical();
}

/* Does not free the memory where the value resides */
bool remove(List* list, void* value) {
    enterCritical();
    if (list->size == 1) {
        free((uint32_t*)list->first);
        list->first = 0;
        list->last = 0;
        list->size = 0;
        leaveCritical();
        return true;
    }

    Node* currentNode = list->first;
    Node* previousNode = 0;
    Node* toRemove = 0;
    
    while(currentNode != 0) {
        // remove process from process list
        if (currentNode->value == value) {
            toRemove = currentNode;
            if (previousNode == 0) {
                list->first = currentNode->next;
            } else if (currentNode->next == 0) {
                list->last = previousNode;
            } else {
                previousNode->next = currentNode->next;
            }
            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (toRemove != 0) {
        list->size--;
        free((uint32_t*)toRemove);
        leaveCritical();
        return true;
    }
    leaveCritical();
    return false;
}

int size(List* list) {
    return list->size;
}

void* get(List* list, int index) {
    enterCritical();
    if (index >= list->size) {
        leaveCritical();
        return 0;
    }

    Node* node = list->first;
    while (index > 0) {
        node = node->next;
        index--;
    }
    leaveCritical();
    return node->value;    
}

int find(List* list, void* value) {
    enterCritical();
    int index = 0;
    Node* node = list->first;
    while (node != 0) {
        if (node->value == value) {
            leaveCritical();
            return index;
        }
        node = node->next;
        index++;
    }
    leaveCritical();
    return -1;
}