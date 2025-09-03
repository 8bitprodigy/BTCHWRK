#ifndef BTCHWRK_SEQUENCE_PRVT_H
#define BTCHWRK_SEQUENCE_PRVT_H

#include "sequence.h"


typedef struct
Seq
{
	void   *data;       /* ALWAYS first */
	size_t  length;     /* ALWAYS second */
	size_t  capacity;
	size_t  datum_size;
};


#endif /* BTCHWRK_SEQUENCE_PRVT_H */
