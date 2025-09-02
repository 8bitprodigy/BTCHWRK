#include <string.h>

#include "_sequence_.h"
#include "text.h"


#define NULL_TERMINATE( txt ) do{ Sequence_add( (txt), '\0'); txt->length--;}while(0)


/*******************************
	Constructor / Destructor
*******************************/
Text
Text_new(char *str, size_t length)
{
	Text txt = (Text)Sequence_new(1, length);
	
	Sequence_append((Sequence)txt, str, length);

	return txt;
}


void
Text_free(Text self)
{
	Sequence_free((Sequence)self);
}


/**************
	Methods
**************/
int
Text_compare(Text self, Text other)
{
	if (self->length != other->length) {
		return (self->length < other->length) ? -1 : 1;
	}

	return memcmp(self->data, other->data, self->length);
}


void
TextBuffer_concat(Text self, Text other)
{
	Sequence_concat((Sequence)self, (Sequence)other);
	NULL_TERMINATE(self);
}


size_t
Text_length(Text self)
{
	return self->length;
}


void
TextBuffer_nConcat(Text self, Text other, size_t length)
{
	Sequence_append((Sequence)self, other->data, length);
	NULL_TERMINATE(self);
}


char *
Text_toString(Text self)
{
	NULL_TERMINATE(self);
	return *((char *)self);
}
