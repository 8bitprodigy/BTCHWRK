#ifndef BTCHWRK_HASHSET_H
#define BTCHWRK_HASHSET_H


#include <stdbool.h>
#include <stddef.h>

#include "common.h"
#include "sequence.h"


typedef struct Hash Hash;
typedef Hash *HashSet;

typedef size_t (*HashFunction)(const void *data, size_t size);


/* Constructor / Destructor */
HashSet HashSet_new( size_t datum_size, HashFunction hash_function);
void    HashSet_free(HashSet self);

/* Member functions */
void    HashSet_add(   HashSet self, const void *data);
void    HashSet_remove(HashSet self, const void *data);
bool    HashSet_has(   HashSet self, const void *data);
void    HashSet_clear( HashSet self);


#endif /* BTCHWRK_HASHSET_H */
