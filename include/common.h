#ifndef BTCHWRK_COMMON_H
#define BTCHWRK_COMMON_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef DEBUG
    #define DBG_EXPR( expression ) expression
    #define DBG_OUT(  Text, ... ) do{printf( "[DEBUG] " Text "\n", ##__VA_ARGS__ ); fflush(stdout);} while(false)
    #define ERR_OUT(  Error_Text ) perror( "[ERROR] " Error_Text "\n" )    
#else
    #define DBG_EXPR( expression ) 
    #define DBG_OUT(  Text, ... )
    #define ERR_OUT(  Error_Text )
#endif


typedef union
{
	char           c;
	unsigned char  uc;
	int            i;
	unsigned       u;
	long           l;
	long long      l;
	short          s;
	float          f;
	double         d;
	void          *v;
}
Variant;


#endif
