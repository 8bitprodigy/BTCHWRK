#ifndef BTCHWRK_SEQUENCE_PRVT_H
#define BTCHWRK_SEQUENCE_PRVT_H

#include "sequence.h"


typedef struct
Seq
{
	size_t  datum_size;
	size_t  length;
	size_t  capacity;
	void   *data;
}
Seq;


#endif /* BTCHWRK_SEQUENCE_PRVT_H */
