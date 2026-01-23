#ifndef LinkedList_h
#define LinkedList_h

#include "../Common/Node.h"

struct LinkedList {
    struct Node *node;
    int lenght;

    void (*insert)(struct LinkedList *linked_list, int index, void *data, unsigned long size);
};

#endif