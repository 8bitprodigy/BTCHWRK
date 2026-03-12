#ifndef BTCHWRK_HASHMAP_H
#define BTCHWRK_HASHMAP_H


#include <stdbool.h>
#include <stddef.h>

#include "common.h"
#include "sequence.h"


typedef struct Hash Hash;
typedef Hash* HashMap;

typedef size_t (*HashFunction)  (const void *key, size_t size);
typedef int    (*CmpFunction)   (const void *a,   const void *b, size_t size);


/* Constructor / Destructor */
HashMap HashMap_new( 
	size_t       key_size, 
	size_t       value_size,
	HashFunction hash, 
	CmpFunction  cmp
);
void    HashMap_free(HashMap self);

/* Member functions */
void    HashMap_put(   HashMap self, const void *key, const void *value);
void    HashMap_remove(HashMap self, const void *key);
void   *HashMap_get(   HashMap self, const void *key);
bool    HashMap_has(   HashMap self, const void *key);
void    HashMap_clear( HashMap self);


#endif /* BTCHWRK_HASHMAP_H */
