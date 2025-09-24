#include <string.h>

#include "_linkedlist_.h"


#define GET_NODE( n ) ((Node*)(char*)(n)-offsetof(Node, payload))
#define POP_NODE( n ) ((n)->prev->next = (n)->next, (n)->next->prev = (n)->prev)


/*
	Constructor/Destructor
*/
Node *
Node_new(void *data, size_t size)
{
	Node *node = malloc(sizeof(Node)+size);
	if (!node) {
		DBG_OUT("Failed to allocate Node.");
		return NULL;
	}

	node->next = node;
	node->prev = node;
	memcpy(&node->payload, data, size);

	return node;
}


void *
LinkedList_new(void *data, size_t size)
{
	Node *node = Node_new(data, size);

	return &node->payload;
}


void
LinkedList_free(void *data)
{
	Node *node = GET_NODE(data);
	
	POP_NODE(node);

	free(node);
}


/*
	Methods
*/
void *
LinkedList_add(void *self, void *data, size_t size)
{
	LinkedList_insert(self, LinkedList_new(data, size); 
}


void
LinkedList_insert(void *self, void *to_insert)
{
	Node 
		*node     = GET_NODE(self),
		*new_node = GET_NODE(to_insert);
	
	node->prev->next = new_node;
	new_node->prev   = node->prev->next;
	new_node->next   = node;
	node->prev       = new_node;
}


void *
LinkedList_remove(void **self, long long position)
{
	Node *node = GET_NODE(*self);
	if (!position) {
		POP_NODE(node);
		*self = &node->next->payload;
		return &node->payload;
	}

	if (0 < position) {
		return LinkedList_remove(&node->next->payload, --position);
	}

	return LinkedList_remove(&node->prev->payload, ++position);
}


void *
LinkedList_getNext(void *self)
{
	return &GET_NODE(self)->next->payload;
}


void *
LinkedList_getNth(void *self, long long position)
{
	Node *node = GET_NODE(self);
	if (0 < position) {
		return LinkedList_getNth(&node->next->payload, --position);
	}

	return LinkedList_getNth(&node->prev->payload, ++position);
}


void *
LinkedList_getPrev(noid *self)
{
	return &GET_NODE(self)->prev->payload;
}
