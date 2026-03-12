#ifndef BTCHWRK_HASHSET_PRVT_H
#define BTCHWRK_HASHSET_PRVT_H


#include "_sequence_.h"
#include "dynamicarray.h"
#include "hashset.h"


typedef struct
FreeRun
{
    size_t start;
    size_t end;
}
FreeRun;

typedef struct
HashBucket
{
	size_t next;
	bool
				  occupied,
				  tombstone;
	unsigned char data[];
}
HashBucket;

typedef struct
Hash
{
	HashFunction  hash;
	HashBucket   *table;
    FreeRun      *free_list;
	size_t        
				  datum_size,
				  bucket_size;
	float         load;
	bool          dirty;
}
Hash;


static inline HashBucket *
bucket_at(HashSet self, size_t index)
{
	return (HashBucket*)(
			(unsigned char*)self->table
			+index
			* self->bucket_size
		);
}


#endif /* BTCHWRK_HASHSET_PRVT_H */
