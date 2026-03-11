#include <stdlib.h>
#include <string.h>

#include "_hashSet_.h"


/***********************
	Internal helpers
***********************/
/* Forward declarations */
static size_t grow_and_rehash(HashSet self);
void HashSet_add(HashSet self, const void *data);

/*  Find the earliest free slot from the free list.
	Returns SIZE_MAX if the free list is empty.
*/
static size_t
free_list_pop_earliest(HashSet self)
{
	if (!self->free_list || DynamicArray_length(self->free_list) == 0)
		return SIZE_MAX;

	size_t
		best_idx   = SIZE_MAX,
		best_start = SIZE_MAX,
		count      = DynamicArray_length(self->free_list);

	for (size_t i = 0; i < count; i++) {
		FreeRun *run = &self->free_list[i];
		if (run->start < best_start) {
			best_start = run->start;
			best_idx   = i;
		}
	}

	if (best_idx == SIZE_MAX)
		return SIZE_MAX;

	FreeRun *run  = &self->free_list[best_idx];
	size_t   slot = run->start;

	if (run->start == run->end) {
		/* Run exhausted, so we remove it */
		DynamicArray_delete(self->free_list, best_idx, 1);
	}
	else {
		run->start++;
	}

	return slot;
}

/* Push a freed index onto the free list as a single-slot run.
*/
static void
free_list_push(HashSet self, size_t index)
{
	size_t count = DynamicArray_length(self->free_list);

	for (size_t i = 0; i < count; i++) {
		FreeRun *run = &self->free_list[i];

		if (index == run->start - 1) {
			run->start = index;
			return;
		}

		if (index == run->end + 1) {
			run->end = index;
			return;
		}
	}

	/* No adjacent run found, add new single-slot run */
	FreeRun run = {index, index};
	DynamicArray_add(self->free_list, &run);
}

/*  Get or allocate a slot for a new chain entry.
    Returns the index of the slot, or SIZE_MAX if a grow+rehash occurred
    (in which case the caller must restart its operation).
*/
static size_t
alloc_slot(HashSet self, HashBucket **bucket_out)
{
	size_t index = free_list_pop_earliest(self);

	if (index == SIZE_MAX) {
		size_t old_cap = DynamicArray_capacity(self->table);
		size_t new_cap = grow_and_rehash(self);

		if (new_cap > old_cap) {
			/* Table was rebuilt — caller must restart */
			*bucket_out = NULL;
			return SIZE_MAX;
		}

		/* Shouldn't happen, but fall back to append */
		HashBucket *nb = calloc(1, self->bucket_size);
		nb->next      = SIZE_MAX;
		nb->occupied  = false;
		nb->tombstone = false;
		DynamicArray_append((void **)&self->table, nb, 1);
		free(nb);
		index = DynamicArray_length(self->table) - 1;
	}

	*bucket_out = bucket_at(self, index);
	return index;
}


/* Grow the table and rehash all live entries into the new capacity. */
static size_t
grow_and_rehash(HashSet self)
{
	size_t old_cap    = DynamicArray_capacity(self->table);
	size_t datum_size = self->datum_size;

	/* Collect all live entries */
	void  *entries = malloc(old_cap * datum_size);
	size_t count   = 0;
	for (size_t i = 0; i < old_cap; i++) {
		HashBucket *b = bucket_at(self, i);
		if (b->occupied && !b->tombstone) {
			memcpy((char*)entries + count * datum_size, b->data, datum_size);
			count++;
		}
	}

	/* Grow the backing array */
	DynamicArray_grow((void **)&self->table);
	size_t new_cap = DynamicArray_capacity(self->table);

	/* Initialize every slot */
	for (size_t i = 0; i < new_cap; i++) {
		HashBucket *b = bucket_at(self, i);
		b->next      = SIZE_MAX;
		b->occupied  = false;
		b->tombstone = false;
	}

	/* Set length = new_cap so overflow appends land beyond primary range */
	HashBucket dummy = {0};
	dummy.next = SIZE_MAX;
	DynamicArray_replace((void **)&self->table, new_cap - 1, &dummy, 1);

	/* Clear free list */
	DynamicArray_clear(self->free_list);

	/* Re-insert all live entries */
	for (size_t i = 0; i < count; i++)
		HashSet_add(self, (char*)entries + i * datum_size);

	free(entries);
	return new_cap;
}


/*
	Constructor / Destructor
*/
HashSet
HashSet_new(size_t datum_size, HashFunction hash_function)
{
	HashSet self = malloc(sizeof(Hash));
	if (!self) {
		DBG_OUT("Failed to allocate HashSet.");
		return NULL;
	}

	self->hash        = hash_function;
	self->datum_size  = datum_size;
	self->bucket_size = sizeof(HashBucket) + datum_size;
	self->load        = 0.0f;
	self->dirty       = false;
	self->table       = DynamicArray_new(self->bucket_size, 8);
	self->free_list   = DynamicArray(FreeRun, 8);

	if (!self->table || !self->free_list) {
		DynamicArray_free(self->table);
		DynamicArray_free(self->free_list);
		free(self);
		return NULL;
	}

	/* Initialize all buckets */
	HashBucket bucket = {0};
	bucket.next = SIZE_MAX;

	size_t cap = DynamicArray_capacity(self->table);
	for (size_t i = 0; i < cap; i++)
		DynamicArray_append((void**)&self->table, &bucket, 1);

	return self;
}

void
HashSet_free(HashSet self)
{
	DynamicArray_free(self->table);
	DynamicArray_free(self->free_list);
	free(self);
}


/*
	Member Functions
*/
void
HashSet_add(HashSet self, const void *data)
{
	self->dirty = true;

restart:;
	size_t
		capacity = DynamicArray_capacity(self->table),
		index    = self->hash(data, self->datum_size) % capacity;
	HashBucket *bucket = bucket_at(self, index);

	/* Unoccupied, fill it */
	if (!bucket->occupied) {
		bucket->occupied  = true;
		bucket->tombstone = false;
		bucket->next      = SIZE_MAX;
		memcpy(bucket->data, data, self->datum_size);
		self->load = (float)DynamicArray_length(self->table) / (float)capacity;
		return;
	}

	/* Walk the chain */
	while (true) {
		/* Already in set, do nothing */
		if (!bucket->tombstone &&
			memcmp(bucket->data, data, self->datum_size) == 0)
			return;

		/* End of chain, append new bucket */
		if (bucket->next == SIZE_MAX) {
			HashBucket *new_bucket;
			size_t new_index = alloc_slot(self, &new_bucket);

			/* grow_and_rehash occurred — all positions changed, restart */
			if (new_index == SIZE_MAX)
				goto restart;

			new_bucket->occupied  = true;
			new_bucket->tombstone = false;
			new_bucket->next      = SIZE_MAX;
			memcpy(new_bucket->data, data, self->datum_size);

			/* Re-walk to end of chain to link,
			   refresh after possible resize */
			bucket = bucket_at(self, index);
			while (bucket->next != SIZE_MAX)
				bucket = bucket_at(self, bucket->next);
			bucket->next = new_index;

			self->load = (float)DynamicArray_length(self->table) / (float)capacity;
			return;
		}

		bucket = bucket_at(self, bucket->next);
	}
}

bool
HashSet_has(HashSet self, const void *data)
{
	size_t 
		capacity = DynamicArray_capacity(self->table),
		index    = self->hash(data, self->datum_size) % capacity;
	HashBucket *bucket = bucket_at(self, index);
	if (!bucket->occupied)
		return false;

	while (true) {
		if (!bucket->tombstone &&
			memcmp(bucket->data, data, self->datum_size) == 0)
			return true;

		if (bucket->next == SIZE_MAX)
			return false;

		bucket = bucket_at(self, bucket->next);
	}

	return false;
}

void
HashSet_remove(HashSet self, const void *data)
{
	size_t 
		capacity  = DynamicArray_capacity(self->table),
		index     = self->hash(data, self->datum_size) % capacity,
		cur_index = index;
	HashBucket
		*bucket = bucket_at(self, index),
		*prev   = NULL;
        
	if (!bucket->occupied)
		return;

	while (true) {
		if (!bucket->tombstone &&
			memcmp(bucket->data, data, self->datum_size) == 0) {

			/* Save next before we wipe the bucket */
			size_t saved_next = bucket->next;

			if (prev)
				prev->next = saved_next;

			memset(bucket, 0, self->bucket_size);
			bucket->next      = saved_next;
			bucket->occupied  = false;
			bucket->tombstone = true;
			if (cur_index != index)
				free_list_push(self, cur_index);

			self->load  = (float)DynamicArray_length(self->table) / (float)capacity;
			self->dirty = true;
			return;
		}

		if (bucket->next == SIZE_MAX)
			return;

		prev      = bucket;
		cur_index = bucket->next;
		bucket    = bucket_at(self, cur_index);
	}
}

void
HashSet_clear(HashSet self)
{
	size_t cap = DynamicArray_capacity(self->table);

	for (size_t i = 0; i < cap; i++) {
		HashBucket *b = bucket_at(self, i);
		b->occupied   = false;
		b->tombstone  = false;
		b->next       = SIZE_MAX;
		memset(b->data, 0, self->datum_size);
	}

	/* Replace free list with one run covering the whole table */
	DynamicArray_clear(self->free_list);
	if (cap > 0) {
		FreeRun run = {0, cap - 1};
		DynamicArray_add(self->free_list, &run);
	}

	self->load  = 0.0f;
	self->dirty = false;
}
