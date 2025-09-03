#include <string.h>

#include "_sequence_.h"
#include "text.h"


#define NULL_TERMINATE( txt ) do{ Sequence_add( (txt), '\0'); txt->length--;}while(0)


/***********
	Text
***********/
char
Text_charAt(Text txt, size_t index)
{
	return (txt) ? ((index < txt->length) ? txt->data[index] : 0) : 0
}


int
Text_compare(Text self, Text other)
{
	if (self->length != other->length) {
		return (self->length < other->length) ? -1 : 1;
	}

	return memcmp(self->data, other->data, self->length);
}


int
Text_find(Text self, Text needle)
{
	if (!self || !needle || !self->data || !needle->data) {
        return -1;
    }
    if (needle->length == 0)            return  0;
    if (needle->length >  self->length) return -1;
      

	for (int i = 0; i < self->length - needle->length; i++) {
		int j;
		for (j = 0; j < needle->length; j++ )
			if (self->data[i+j] != needle->data[j]) break;

		if (j == needle->length) return i;
	}

	return -1;
}


int
Text_findChar(Text self, char c)
{
	if (!self) return -1;
	
	for (int i = 0; i < self->length; i++) {
		if (self->data[i] == c) return i;
	}

	return -1;
}


size_t
Text_length(Text self)
{
	return self ? self->length : 0;
}


char *
Text_toCStr(Text self)
{
	return self ? self->data : NULL;
}


/*****************
	TextBuffer
*****************/
/*
	Constructor / Destructor
*/
TextBuffer
TextBuffer_new(size_t capacity)
{
	TextBuff buf = (TextBuffer)Sequence_new(sizeof(char), capacity);

	return buf;
}


TextBuffer
TextBuffer_fromText(Text self)
{
	TextBuff buf = TextBuffer_new(self->length);
	Sequence_append((Sequence)buf, self, length);
	
	return buf;
}


TextBuffer
TextBuffer_fromCStr(const char *cstr)
{
	TextBuff buf    = TextBuffer_new(sizeof(cstr)-1);
	Sequence_append((Sequence)buf, cstr, sizeof(cstr));

	return buf;
}


void
TextBuffer_free(Text self)
{
	Sequence_free((Sequence)self);
}


/*
	Methods
*/
/* Access */
size_t
TextBuffer_length(TextBuffer self)
{
	return self ? self->length : 0;
}

/* Mutators */
void
TextBuffer_insert(TextBuffer self, size_t index, Text txt)
{
	Sequence_insert((Sequence)self, index, txt->data, txt->length);
}


void
TextBuffer_concat(TextBuffer self, Text other)
{
	Sequence_concat((Sequence)self, (Sequence)other);
	NULL_TERMINATE(self);
}


void
TextBuffer_nConcat(TextBuffer self, Text other, size_t length)
{
	Sequence_append((Sequence)self, other->data, length);
	NULL_TERMINATE(self);
}


void
TextBuffer_setChar(TextBuffer self, char c, size_t index)
{
	if (!self) return;
	char *entry = Sequence_at(self, index);
	if (entry) *entry = c;
}
