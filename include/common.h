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

typedef union Any
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

	char                ca[sizeof(Any) / sizeof(char)];
    unsigned char       uca[sizeof(Any) / sizeof(unsigned char)];
    
    /* These will have arrays if they're smaller than the largest type */
    #if (sizeof(Any) / sizeof(short)) > 1
    short               sa[sizeof(Any) / sizeof(short)];
    unsigned short      usa[sizeof(Any) / sizeof(unsigned short)];
    #endif
    
    #if (sizeof(Any) / sizeof(int)) > 1
    int                 ia[sizeof(Any) / sizeof(int)];
    unsigned            ua[sizeof(Any) / sizeof(unsigned)];
    #endif
    
    #if (sizeof(Any) / sizeof(long)) > 1
    long                la[sizeof(Any) / sizeof(long)];
    unsigned long       ula[sizeof(Any) / sizeof(unsigned long)];
    #endif
    
    #if (sizeof(Any) / sizeof(long long)) > 1
    long long           lla[sizeof(Any) / sizeof(long long)];
    unsigned long long  ulla[sizeof(Any) / sizeof(unsigned long long)];
    #endif
    
    #if (sizeof(Any) / sizeof(float)) > 1
    float               fa[sizeof(Any) / sizeof(float)];
    #endif
    
    #if (sizeof(Any) / sizeof(double)) > 1
    double              da[sizeof(Any) / sizeof(double)];
    #endif
    
    #if (sizeof(Any) / sizeof(void*)) > 1
    void               *va[sizeof(Any) / sizeof(void*)];
    #endif
}
Any;

typedef struct
{
	VariantTag tag;
	Any        value;
}
Variant;


#endif
