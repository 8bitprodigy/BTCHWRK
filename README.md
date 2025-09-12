# BTCHWRK

Pronounced "Batch Work" ;).

---

This is a library of common useful data types, containers and algorithms one may 
need that isn't part of the C standard library, provided to you, completely for
free as public domain or under the terms of the Zero BSD license -- take your 
pick.

---
## Features/API:

- `Any` - `<btchwrk/common.h>`: A typedef'd union that can hold any basic C type.

- `Sequence` - `<btchwrk/sequence.h>`: A dynamic array container type.
	- **Methods:**
		- `Sequence Sequence_new( size_t datum_size, size_t capacity)`: Creates 
		a new `Sequence` whose elements are `datum_size` wide and with `capacity` 
		number of elements.
		- `void Sequence_free(Sequence seq)`: Grees a `Sequence` from memory.
		- `void Sequence_grow(Sequence seq)`: Grows the capacity of the `Sequence`
		by `Sequence.capacity * SEQUENCE_GROWTH_FACTOR` amount.
		- `void Sequence_shrink(Sequence seq)`: Shrinks the capacity of the 
		`Sequence` to be equal to its length.
		- `void Sequence_add(Sequence seq, void *datum)`: Adds an element, `datum`
		to the end of the `Sequence`.
		- `void *Sequence_at(Sequence seq, size_t index)`: Returns a pointer to 
		the data at `index`.
		- `void Sequence_append(Sequence seq, void *data, size_t size)`: Appends
		an array, `data`, of `size` elements in length to the end of the 
		`Sequence`.
		- `size_t Sequence_capacity(Sequence seq)`: Returns the total capacity 
		of a `Sequence`.
		- `void Sequence_concat(Sequence seq1, Sequence seq2)`: Concatenates two
		`Sequence`s together.
		- `void Sequence_delete(Sequence seq, size_t index, size_t size)`: 
		Deletes `size` number of elements from a `Sequence`, starting at `index`.
		- `void Sequence_insert(Sequence seq, size_t index, void *data, size_t size)`
		: Inserts an array, `data`, `size` elements in length into a `Sequence`
		starting at `index`, pushing the elements starting at `index` and after 
		back.
		- `Size_t Sequence_length(Sequence seq)`: Returns the number of elements
		in a `Sequence`.
		- `void Sequence_replace(Sequence seq, size_t index, void *data, size_t size)`
		: Replaces `size` number of elements of a `Sequence`, starting at `index`
		with `size` number of elements from array `data`. 

- `Text` = `<btchwrk/text.h>`: A length-counted constant string type.
	- **Macros:**
		- `TXT( literal )`: Wraps a string literal into a `Text` literal.
		- `Text( literal )`: Alias of `TXT()` to be  used as like a constructor.
		- `CSTR2TXT( cstr )`: Wraps a C string into a `Text` literal.
	- **Methods:**
		- `char Text_charAt(Text txt, size_t index)`: Returns the character at a
		particular index.
		- `int Text_compare(Text txt1, Text txt2)`: Compare two strings 
		lexicographically. The sign of the result is the difference between the
		string values. Returns 0 if equal.
		- `int Text_find(Text txt, Text needle)`: Finds the first occurrance of 
		the `Text` `needle` in `txt` and returns the starting position of that.
		- `int Text_findChar(Text txt, char c)`: Finds the first occurrance of 
		`char` `c` in `txt`.
		- `size_t Text_length(Text txt)` Returns the length of `txt`.
		- `int Text_rfind(Text txt, Text needle)`: Reverse finds the first
		occurrance of `Text` `needle` in `txt` and returns the starting position.
		- `char *Text_toCStr(Text txt)`: Returns the C string equivalent of 
		`txt`.

- `TextBuffer` = `<btchwrk/text.h>`: A length-counted dynamic string type.
	- **Macros:**
		- `TB2TXT( buf )`: Casts a `TextBuffer` to a `Text`.
		- `TB2STR( buf )`: Returns a C string from a `TextBuffer`.
	- **Constructors:**
		- `TextBuffer TextBuffer_new(size_t capacity)`: Constructs a new
		`TextBuffer`, preallocated with `capacity` characters.
		- `TextBuffer TextBuffer_fromText(Text txt)`: Constructs a new 
		`TextBuffer` from a pre-existing `Text`.
		- `TextBuffer TextBuffer_fromCStr(const char *cstr)`: Constructs a
		new `TextBuffer` from a C string.
		- `TextBuffer TextBuffer_substring(TextBuffer buf, size_t start, size_t length)`:
		Constructs a new `TextBuffer` from the substring of `buf` starting at
		`start` for `length` characters long.
	- **Destructor:**
		- `void TextBuffer_free(TextBuffer buf)`: Destructs `buf`, freeing it 
		from memory.
	- **Methods:**
		- **Accessors:**
			- `size_t TextBuffer_length(TextBuffer buf)`: Returns the length of
			a `TextBuffer`.
		- **Mutators:**
			- `void TextBuffer_clear(TextBuffer buf)`: Clears the text from the
			`TextBuffer`.
			- `void TextBuffer_concat(TextBuffer buf, Text txt)`: Concatenates
			a `Text` to the end of `TextBuffer`.
			- `void TextBuffer_nConcat(TextBuffer buf, Text txt, size_t length)`:
			Concatenates `length` amount of `txt` to a `TextBuffer`.
			- `void TextBuffer_insert(TextBuffer buf, size_t index, Text txt)`:
			Inserts `txt` into `buf` at `index`.
			- `void TextBuffer_setChar(TextBuffer buf, char c, size_t index)`:
			Sets the `char` at `index` in `buf` to `c`.
		
	
- `Variant` - `<btchwrk/common.h>`: A transparent tagged union which can hold 
any basic C type. Has the following structure:
	- `VariantTag tag`: The tag indicating the currently held type.
	- `Any value`: The value held in the `Variant`.

- `VariantTag` - `<btchwrk/common.h>`: A typedef'd enum, enumerating every basic
C type. Eat entry is in `SHOUT_CASE` and starts with the `VARIANT_` prefix.

---

## License:

Zero-BSD

```
Copyright (C) 2025 by chrisxdeboy@gmail.com

Permission to use, copy, modify, and/or distribute this software for any purpose 
with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH 
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND 
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, 
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER 
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF 
THIS SOFTWARE.
```
