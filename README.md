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
	- `Sequence Sequence_new( size_t datum_size, size_t capacity)`: Creates a new
	`Sequence` whose elements are `datum_size` wide and with `capacity` number
	of elements.
	- `void Sequence_free(Sequence seq)`: Grees a `Sequence` from memory.
	- `void Sequence_grow(Sequence seq)`: Grows the capacity of the `Sequence`
	by `Sequence.capacity * SEQUENCE_GROWTH_FACTOR` amount.
	- `void Sequence_shrink(Sequence seq)`: Shrinks the capacity of the 
	`Sequence` to be equal to its length.
	- `void Sequence_add(Sequence seq, void *datum)`: Adds an element, `datum`
	to the end of the `Sequence`.
	- `void *Sequence_at(Sequence seq, size_t index)`: Returns a pointer to the
	data at `index`.
	- `void Sequence_append(Sequence seq, void *data, size_t size)`: Appends an
	array, `data`, of `size` elements in length to the end of the `Sequence`.
	- `size_t Sequence_capacity(Sequence seq)`: Returns the total capacity of a
	`Sequence`.
	- `void Sequence_concat(Sequence seq1, Sequence seq2)`: Concatenates two
	`Sequence`s together.
	- `void Sequence_delete(Sequence seq, size_t index, size_t size)`: Deletes
	`size` number of elements from a `Sequence`, starting at `index`.
	- `void Sequence_insert(Sequence seq, size_t index, void *data, size_t size)`
	: Inserts an array, `data`, `size` elements in length into a `Sequence`
	starting at `index`, pushing the elements starting at `index` and after back.
	- `Size_t Sequence_length(Sequence seq)`: Returns the number of elements in
	a `Sequence`.
	- `void Sequence_replace(Sequence seq, size_t index, void *data, size_t size)`
	: Replaces `size` number of elements of a `Sequence`, starting at `index`
	with `size` number of elements from array `data`. 

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
