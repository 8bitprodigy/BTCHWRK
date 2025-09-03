#include <string.h>

#include "_sequence_.h"


#define INDEX( i ) (char*)self->data + ((i) * self->datum_size)


/*******************************
    Constructor / Destructor
*******************************/
Sequence
Sequence_new( size_t datum_size, size_t capacity )
{
	Sequence seq   = malloc(sizeof(Seq));
	if (!seq) {
		DBG_OUT("Failed to allocate Sequence.");
		return NULL;
	}
	
	seq->data = NULL;
	seq->data = malloc(datum_size*capacity);
	if (!seq->data) {
		DBG_OUT("Failed to allocate Sequence.data.");
		free(seq);
		return NULL;
	}
	
	seq->datum_size = datum_size;
	seq->length     = 0;
	seq->capacity   = capacity;

	return seq;
}


void 
Sequence_free(Sequence self)
{
	free(self->data);
	free(self);
}


void
Sequence_grow(Sequence self)
{
	assert(self);
	
	size_t  new_len  = SEQUENCE_GROWTH_FACTOR * self->capacity;
	size_t  new_size = new_len * self->datum_size;
	void   *new_arr  = realloc(self->data, new_size);
	if (!new_arr) {
		DBG_OUT("Failed to allocate new grown array.");
		return;
	}

	self->data     = new_arr;
	self->capacity = new_len;
}


void
Sequence_shrink(Sequence self)
{
	assert(self);
	
	void *new_arr = malloc((self->length + 1) * self->datum_size);
	if (!new_arr) {
		DBG_OUT("Failed to allocate new shrunk array.");
		return;
	}

	memcpy(new_arr, self->data, self->length * self->datum_size);

	free(self->data);

	self->data     = new_arr;
	self->capacity = self->length;
}


void
Sequence_add(Sequence self, void *datum)
{
	Sequence_append(self, datum, 1);
}


void
Sequence_append(Sequence self, void *data, size_t size)
{
	assert(self);
	assert(data);

	while (self->capacity <= self->length + size)
		Sequence_grow(self);

	memcpy(INDEX(self->length), data, size * self->datum_size);
	self->length += size;
}


void *
Sequence_at(Sequence self, size_t index)
{
	assert(self);
	assert(index < self->length);

	return INDEX(index);
}


size_t
Sequence_capacity(Sequence self)
{
	assert(self);

	return self->capacity;
}


void
Sequence_concat(Sequence self, Sequence seq)
{
	Sequence_append(self, seq->data, seq->length);
}


void
Sequence_delete(Sequence self, size_t index, size_t size)
{
	assert(self);
	assert(index < self->capacity);
	assert(index < self->length);
	assert(size);
	
	int rest = self->length - index;
	memcpy(INDEX(index), INDEX(index + size), rest * self->datum_size);
	self->length -= size;
}


void
Sequence_insert(Sequence self, size_t index, void *data, size_t size)
{
	assert(self);
	assert(index < self->capacity);
	assert(index < self->length);
	assert(data);
	assert(size);

	while (self->capacity <= self->length + size)
		Sequence_grow(self);

	int rest = self->length - index;
	memcpy(INDEX(index+size), INDEX(index), rest * self->datum_size);
	memcpy(INDEX(index), data, size * self->datum_size);
	self->length += size;
}


size_t
Sequence_length(Sequence self)
{
	assert(self);
	
	return self->length;
}


void
Sequence_replace(Sequence self, size_t index, void *data, size_t size)
{
	assert(self);
	assert(index < self->capacity);
	assert(index < self->length);
	assert(data);
	assert(size);

	int amount = index + size;
	while (self->capacity <= amount)
		Sequence_grow(self);

	memcpy(INDEX(index), data, size);
	self->length = (self->length < amount) ? amount : self->length;
}
