#ifndef BTCHWRK_LINKEDLIST_PRVT_H
#define BTCHWRK_LINKEDLIST_PRVT_H


#include "linkedlist.h"


typedef struct
Node
{
	struct Node *next;
	struct Node *prev;
	char         payload[];
}
Node;


#endif /* BTCHWRK_LINKEDLIST_PRVT_H */
