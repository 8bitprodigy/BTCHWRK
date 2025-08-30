#ifndef BTCHWRK_SEQUENCE_H
#define BTCHWRK_SEQUENCE_H

#include "common.h"


#ifndef SEQUENCE_GROWTH_FACTOR
	#define SEQUENCE_GROWTH_FACTOR 2.0f
#endif


typedef struct Seq Seq;       /* Sequence type forward decl */
typedef struct Seq *Sequence; /* Pointer to the sequence */


/* Constructor / Destructor */
Seq   *Sequence_new(      size_t     datum_size, size_t capacity);
void   Sequence_free(     Sequence   seq);

/* Methods */
void   Sequence_grow(     Sequence   seq);
void   Sequence_shrink(   Sequence   seq);

void    Sequence_add(     Sequence   seq,  void     *datum);
void   *Sequence_at(      Sequence   seq,  size_t    index);
void    Sequence_append(  Sequence   seq,  void     *data,  size_t  size);
size_t  Sequence_capacity(Sequence   seq);
void    Sequence_concat(  Sequence   seq1, Sequence  seq2);
void    Sequence_delete(  Sequence   seq,  size_t    index, size_t size);
void    Sequence_insert(  Sequence   seq,  size_t    index, void   *data,   size_t size);
size_t  Sequence_length(  Sequence   seq);
void    Sequence_replace( Sequence   seq,  size_t    index, void   *data,   size_t size);


#endif /* BTCHWRK_SEQUENCE_H */
