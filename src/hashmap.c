#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_hashmap_.h"


/***********************
	Internal helpers
***********************/
/* Forward declarations */
static size_t grow_and_rehash(HashMap self);
void HashMap_put(HashMap self, const void *key, const void *value);

/* Default comparator — wraps memcmp to match CmpFunction signature */
static int
default_cmp(const void *a, const void *b, size_t size)
{
	return memcmp(a, b, size);
}

/*  Find the earliest free slot from the free list.
	Returns SIZE_MAX if the free list is empty.
*/
static size_t
free_list_pop_earliest(HashMap self)
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
		DynamicArray_delete(self->free_list, best_idx, 1);
	}
	else {
		run->start++;
	}

	return slot;
}

/* Push a freed index onto the free list as a single-slot run. */
static void
free_list_push(HashMap self, size_t index)
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

	FreeRun run = {index, index};
	DynamicArray_add(self->free_list, &run);
}

/*  Get or allocate a slot for a new chain entry.
    Returns the index of the slot, or SIZE_MAX if a grow+rehash occurred
    (in which case the caller must restart its operation).
*/
static size_t
alloc_slot(HashMap self, HashBucket **bucket_out)
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
grow_and_rehash(HashMap self)
{
	size_t old_cap   = DynamicArray_capacity(self->table);
	size_t key_size  = self->key_size;
	size_t val_size  = self->value_size;
	size_t pair_size = key_size + val_size;

	/* Collect all live key-value pairs */
	void  *entries = malloc(old_cap * pair_size);
	size_t count   = 0;
	for (size_t i = 0; i < old_cap; i++) {
		HashBucket *b = bucket_at(self, i);
		if (b->occupied && !b->tombstone) {
			void *dst = (char*)entries + count * pair_size;
			memcpy(dst,              bucket_key(b),        key_size);
			memcpy((char*)dst + key_size, bucket_val(self, b), val_size);
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
	for (size_t i = 0; i < count; i++) {
		void *k = (char*)entries + i * pair_size;
		void *v = (char*)k + key_size;
		HashMap_put(self, k, v);
	}

	free(entries);
	return new_cap;
}


/*
	Constructor / Destructor
*/
bool
HashMap_init(Hash *self, size_t key_size, size_t value_size,
             HashFunction hash, CmpFunction cmp)
{
	self->hash        = hash;
	self->cmp         = cmp ? cmp : default_cmp;
	self->key_size    = key_size;
	self->value_size  = value_size;
	self->bucket_size = sizeof(HashBucket) + key_size + value_size;
	self->load        = 0.0f;
	self->dirty       = false;
	self->table       = DynamicArray_new(self->bucket_size, 8);
	self->free_list   = DynamicArray(FreeRun, 8);

	if (!self->table || !self->free_list) {
		DynamicArray_free(self->table);
		DynamicArray_free(self->free_list);
		return false;
	}

	/* Initialize all buckets */
	HashBucket bucket = {0};
	bucket.next = SIZE_MAX;

	size_t cap = DynamicArray_capacity(self->table);
	for (size_t i = 0; i < cap; i++)
		DynamicArray_append((void**)&self->table, &bucket, 1);

	return true;
}

void
HashMap_destroy(Hash *self)
{
	DynamicArray_free(self->table);
	DynamicArray_free(self->free_list);
}

HashMap
HashMap_new(size_t key_size, size_t value_size,
            HashFunction hash, CmpFunction cmp)
{
	HashMap self = malloc(sizeof(Hash));
	if (!self) {
		DBG_OUT("Failed to allocate HashMap.");
		return NULL;
	}

	if (!HashMap_init(self, key_size, value_size, hash, cmp)) {
		free(self);
		return NULL;
	}

	return self;
}

void
HashMap_free(HashMap self)
{
	HashMap_destroy(self);
	free(self);
}


/*
	Member Functions
*/
void
HashMap_put(HashMap self, const void *key, const void *value)
{
	self->dirty = true;

restart:;
	size_t
		capacity = DynamicArray_capacity(self->table),
		index    = self->hash(key, self->key_size) % capacity;
	HashBucket *bucket = bucket_at(self, index);

	/* Unoccupied, fill it */
	if (!bucket->occupied) {
		bucket->occupied  = true;
		bucket->tombstone = false;
		bucket->next      = SIZE_MAX;
		memcpy(bucket_key(bucket),        key,   self->key_size);
		memcpy(bucket_val(self, bucket),  value, self->value_size);
		self->load = (float)DynamicArray_length(self->table) / (float)capacity;
		return;
	}

	/* Walk the chain */
	while (true) {
		/* Key already present — update value in place */
		if (!bucket->tombstone &&
			self->cmp(bucket_key(bucket), key, self->key_size) == 0) {
			memcpy(bucket_val(self, bucket), value, self->value_size);
			return;
		}

		/* End of chain — append new bucket */
		if (bucket->next == SIZE_MAX) {
			HashBucket *new_bucket;
			size_t new_index = alloc_slot(self, &new_bucket);

			/* grow_and_rehash occurred — all positions changed, restart */
			if (new_index == SIZE_MAX)
				goto restart;

			new_bucket->occupied  = true;
			new_bucket->tombstone = false;
			new_bucket->next      = SIZE_MAX;
			memcpy(bucket_key(new_bucket),        key,   self->key_size);
			memcpy(bucket_val(self, new_bucket),  value, self->value_size);

			/* Re-walk to end of chain to link */
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

/*  Returns a pointer directly into the bucket's value storage,
    or NULL if the key is not present.
    The pointer is valid until the next mutation of the map.
*/
void *
HashMap_get(HashMap self, const void *key)
{
	size_t
		capacity = DynamicArray_capacity(self->table),
		index    = self->hash(key, self->key_size) % capacity;
	HashBucket *bucket = bucket_at(self, index);

	if (!bucket->occupied)
		return NULL;

	while (true) {
		if (!bucket->tombstone &&
			self->cmp(bucket_key(bucket), key, self->key_size) == 0)
			return bucket_val(self, bucket);

		if (bucket->next == SIZE_MAX)
			return NULL;

		bucket = bucket_at(self, bucket->next);
	}
}

bool
HashMap_has(HashMap self, const void *key)
{
	return HashMap_get(self, key) != NULL;
}

void
HashMap_remove(HashMap self, const void *key)
{
	size_t
		capacity  = DynamicArray_capacity(self->table),
		index     = self->hash(key, self->key_size) % capacity,
		cur_index = index;
	HashBucket
		*bucket = bucket_at(self, index),
		*prev   = NULL;

	if (!bucket->occupied)
		return;

	while (true) {
		if (!bucket->tombstone &&
			self->cmp(bucket_key(bucket), key, self->key_size) == 0) {

			size_t saved_next = bucket->next;

			if (prev)
				prev->next = saved_next;

			memset(bucket, 0, self->bucket_size);
			bucket->next      = saved_next;
			bucket->occupied  = false;
			bucket->tombstone = true;

			/* Only recycle overflow nodes, not primary slots */
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
HashMap_clear(HashMap self)
{
	size_t cap = DynamicArray_capacity(self->table);

	for (size_t i = 0; i < cap; i++) {
		HashBucket *b = bucket_at(self, i);
		b->occupied   = false;
		b->tombstone  = false;
		b->next       = SIZE_MAX;
		memset(b->data, 0, self->key_size + self->value_size);
	}

	DynamicArray_clear(self->free_list);
	if (cap > 0) {
		FreeRun run = {0, cap - 1};
		DynamicArray_add(self->free_list, &run);
	}

	self->load  = 0.0f;
	self->dirty = false;
}
