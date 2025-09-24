#ifndef BTCHWRK_HASHSET_H
#define BTCHWRK_HASHSET_H


#include "common.h"
#include "sequence.h"


typedef Hash Hash;
typedef Hash* HashSet;

typedef size_t (HashFunction*)(...);


/* Constructor / Destructor */
HashSet HashSet_new( size_t  datum_size, HashFunction hash_function);
void    HashSet_free(HashSet hash);

/* Member functions */
void HashSet_add(   HashSet hash, void *data);
void HashSet_remove(HashSet hash, void *data);
void HashSet_has(   HashSet hash, void *data);


#endif /* BTCHWRK_HASHSET_H */
