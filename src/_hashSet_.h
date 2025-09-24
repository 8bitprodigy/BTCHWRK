#ifndef BTCHWRK_HASHSET_PRVT_H
#define BTCHWRK_HASHSET_PRVT_H


#include "_sequence_.h"
#include "hashset.h"


typedef struct
Hash
{
	HashFunction hash;
	bool            dirty;
	float           load;
	Seq             table;
}
Hash;


#endif /* BTCHWRK_HASHSET_PRVT_H */
