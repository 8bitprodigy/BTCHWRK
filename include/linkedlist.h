#ifndef BTCHWRK_LINKEDLIST_H
#define BTCHWRK_LINKEDLIST_H


#include "common.h"


typedef struct Node  Node;
typedef struct Node* LinkedList;


/* Constructor/Destructor */
void *LinkedList_new( void *data, size_t size);
void  LinkedList_free(void *data);

/* Methods */
void *LinkedList_add(    void  *node, void      *data,      size_t size); /* Allocates */
void  LinkedList_insert( void  *node, void      *to_insert);
void *LinkedList_remove( void **node, long long  position); /* Reassigns node if head is removed (position==0) */
void *LinkedList_getNext(void  *node);
void *LinkedList_getNth( void  *node, long long  position);
void *LinkedList_getPrev(void  *node);


#endif /* BTCHWRK_LINKEDLIST_H */
