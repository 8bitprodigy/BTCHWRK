#include <string.h>

#include "_sequence_.h"
#include "text.h"

// Right after your includes in text.c:
static Sequence test_var;     // This works
static Sequence* test_var3;   // Does this work?
typedef Sequence TextBuffer_test;
static TextBuffer_test test_var4;  // Does this work?

#define NULL_TERMINATE( txt ) do{ \
    Sequence_add( (txt), &(char){'\0'}); \
    (txt)->length--; \
} while(0)


/***********
	Text
***********/
char
Text_charAt(Text txt, size_t index)
{
	txt.data[index];
} /* Text_charAt */


int
Text_compare(Text self, Text other)
{
	if (self.length != other.length) {
		return (self.length < other.length) ? -1 : 1;
	}

	return memcmp(self.data, other.data, self.length);
} /* Text_compare */


int
Text_find(Text self, Text needle)
{
	if (!self.data || !needle.data) {
        return -1;
    }
    if (needle.length == 0)            return  0;
    if (needle.length >  self.length) return -1;
	
	for (int i = 0; i < self.length - needle.length; i++) {
		int j;
		for (j = 0; j < needle.length; j++ )
			if (self.data[i+j] != needle.data[j]) break;

		if (j == needle.length) return i;
	}

	return -1;
} /* Text_find */


int
Text_findChar(Text self, char c)
{
	for (int i = 0; i < self.length; i++) {
		if (self.data[i] == c) return i;
	}

	return -1;
} /* Text_findChar */


size_t
Text_length(Text self)
{
	return self.length;
} /* Text_length */


int
Text_rfind(Text self, Text needle)
{
	if (!self.data || !needle.data)   return -1;
    if (needle.length == 0)           return  0;
    if (needle.length >  self.length) return -1;
      

	for (int i = self.length - needle.length; 0 < i; i--) {
		int j;
		for (j = 0; j < needle.length; j++ )
			if (self.data[i+j] != needle.data[j]) break;

		if (j == needle.length) return i;
	}

	return -1;
} /* Text_rfind */

char *
Text_toCStr(Text self)
{
	return self.data;
} /* Text_toCStr */


/*****************
	TextBuffer
*****************/
/*
	Constructor / Destructor
*/
TextBuffer
TextBuffer_new(size_t capacity)
{
	TextBuffer buf = (TextBuffer)Sequence_new(sizeof(char), capacity);

	return buf;
} /* TextBuffer_new */


TextBuffer
TextBuffer_fromText(Text txt)
{
	TextBuffer buf = TextBuffer_new(txt.length);
	Sequence_append((Sequence)buf, (void*)txt.data, txt.length);
	
	return buf;
} /* TextBuffer_fromText */


TextBuffer
TextBuffer_fromCStr(const char *cstr)
{
	TextBuffer buf    = TextBuffer_new(sizeof(cstr)-1);
	Sequence_append((Sequence)buf, cstr, sizeof(cstr));

	return buf;
} /* TextBuffer_fromCStr */


void
TextBuffer_free(TextBuffer self)
{
	Sequence_free((Sequence)self);
} /* TextBuffer_free */


/*
	Methods
*/
/* Access */
size_t
TextBuffer_length(TextBuffer self)
{
	return self ? self->length : 0;
} /* TextBuffer_length */

/* Mutators */
void
TextBuffer_clear(TextBuffer buf)
{
	buf->data = '\0';
	buf->length = 0;
} /* TextBuffer_clear */


void
TextBuffer_concat(TextBuffer self, Text other)
{
	TextBuffer_nConcat(self, other, other.length);
} /* TextBuffer_concat */


void
TextBuffer_nConcat(TextBuffer self, Text other, size_t length)
{
	Sequence_append((Sequence)self, other.data, (length <= other.length) ? length : other.length);
	NULL_TERMINATE(self);
} /* TextBuffer_nConcat */


void
TextBuffer_insert(TextBuffer self, size_t index, Text txt)
{
	Sequence_insert((Sequence)self, index, txt.data, txt.length);
	NULL_TERMINATE(self);
} /* TextBuffer_insert */


void
TextBuffer_setChar(TextBuffer self, char c, size_t index)
{
	if (!self) return;
	char *entry = Sequence_at(self, index);
	if (entry) *entry = c;
} /* TextBuffer_setChar */


TextBuffer
TextBuffer_substring(TextBuffer self, size_t start, size_t length)
{
	if (
		!self
		|| self->length < length
		|| self->length < start
		|| self->length < start + length
	) return NULL;

	TextBuffer buf = TextBuffer_new(length);

	TextBuffer_insert(buf, 0, (Text){self->data+start, length}); 
	
	return buf;
} /* TextBuffer_substring */
