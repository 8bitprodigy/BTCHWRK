#ifndef BTCHWRK_COMMON_H
#define BTCHWRK_COMMON_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef DEBUG
    #define DBG_EXPR( expression ) expression
    #define DBG_OUT(  Text, ... ) do{printf( "[DEBUG] " Text "\n", ##__VA_ARGS__ ); fflush(stdout);} while(0)
    #define ERR_OUT(  Error_Text ) perror( "[ERROR] " Error_Text "\n" )    
#else
    #define DBG_EXPR( expression ) 
    #define DBG_OUT(  Text, ... )
    #define ERR_OUT(  Error_Text )
#endif


typedef union
{
	char                c;
	unsigned char       uc;
	int                 i;
	unsigned            u;
	long                l;
	unsigned long       ul;
	long long           ll;
	unsigned long long  ull;
	short               s;
	unsigned short      us;
	float               f;
	double              d;
	void               *v;
}
Any;

typedef enum
{
	VARIANT_CHAR,
	VARIANT_UCHAR,
	VARIANT_INT,
	VARIANT_UNSIGNED,
	VARIANT_LONG,
	VARIANT_ULONG,
	VARIANT_LONG_LONG,
	VARIANT_ULONG_LONG,
	VARIANT_SHORT,
	VARIANT_USHORT,
	VARIANT_FLOAT,
	VARIANT_DOUBLE,
	VARIANT_VOID_PTR,
}
VariantTag;

typedef struct
{
	VariantTag tag;
	Any        value;
}
Variant;


#endif
