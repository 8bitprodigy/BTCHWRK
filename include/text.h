#ifndef BTCHWRK_TEXT_H
#define BTCHWRK_TEXT_H


#include "common.h"
#include "sequence.h"


#define TXT(      txt  ) ((Text){ &(txt), sizeof((txt))-1 })
#define TB2TXT(   buf  ) ((Text){ (char*)*(buf), TextBuffer_length( (buf) )})
#define CSTR2TXT( cstr ) ((Text){(cstr), strlen(cstr))
#define TXT2STR(  txt  ) Text_toCStr( (txt) )

/* Macros to replace equivalent string functions */
#define txtcat( txt1, txt2 ) Text_concat( (txt1), (txt2) )
#define txtncat( txt1, txt2, len ) Text_nConcat( (txt1), (txt2), (length) )
#define txtcmp( txt1, txt2 ) Text_compare( (txt1), (txt2) )


typedef struct
{
	char   *data;
	size_t  length;
}
Text;

typedef Sequence TextBuffer;


int     Text_compare( Text txt1, Text txt2);
char   *Text_toCStr(  Text txt);


/* Constructor / Destructor */
TextBuffer TextBuffer_new(     size_t      capacity);
TextBuffer TextBuffer_fromText(Text        txt);
TextBuffer TextBuffer_fromCStr(const char *cstr);
void       TextBuffer_free(    TextBuffer  buf);

/* 
	TextBuffer Methods 
*/
/* Access */
size_t TextBuffer_length(  TextBuffer buf);
/* Mutators */
void   TextBuffer_insert(  TextBuffer buf, Text txt);
void   TextBuffer_nConcat( TextBuffer buf, Text txt, size_t length);
void   TextBuffer_concat(  TextBuffer buf, Text txt);


#endif /* BTCHWRK_TEXT_H */
