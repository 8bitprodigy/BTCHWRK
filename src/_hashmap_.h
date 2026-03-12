#ifndef BTCHWRK_HASHMAP_PRVT_H
#define BTCHWRK_HASHMAP_PRVT_H


#include "_sequence_.h"
#include "dynamicarray.h"
#include "hashmap.h"


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
	unsigned char data[];   /* [key_size bytes][value_size bytes] */
}
HashBucket;

typedef struct
Hash
{
	HashFunction  hash;
	CmpFunction   cmp;
	HashBucket   *table;
	FreeRun      *free_list;
	size_t
				  key_size,
				  value_size,
				  bucket_size;
	float         load;
	bool          dirty;
}
Hash;


static inline HashBucket *
bucket_at(HashMap self, size_t index)
{
	return (HashBucket*)(
			(unsigned char*)self->table
			+ index
			* self->bucket_size
		);
}

static inline void *
bucket_key(HashBucket *bucket)
{
	return bucket->data;
}

static inline void *
bucket_val(HashMap self, HashBucket *bucket)
{
	return bucket->data + self->key_size;
}


bool HashMap_init(
	Hash         *self, 
	size_t        key_size, 
	size_t        value_size,
	HashFunction  hash, 
	CmpFunction   cmp
);
void HashMap_destroy(Hash *self);


#endif /* BTCHWRK_HASHMAP_PRVT_H */
