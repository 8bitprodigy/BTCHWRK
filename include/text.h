#ifndef BTCHWRK_TEXT_H
#define BTCHWRK_TEXT_H


#include "common.h"
#include "sequence.h"


#define TXT(      literal ) ((Text){(literal), sizeof(literal)-1})
#define Text(     literal ) TXT( literal )
#define CSTR2TXT( cstr    ) ((Text){(cstr), strlen(cstr)})
#define TB2TXT(   buf     ) (*(Text*)(buf))
#define TXT2STR(  txt     ) ((txt).data)
#define TB2STR(   buf     ) TXT2STR(TB2TXT((buf)))

/* Macros to replace equivalent string functions */
#define txtcat( txt1, txt2 ) Text_concat( (txt1), (txt2) )
#define txtncat( txt1, txt2, len ) Text_nConcat( (txt1), (txt2), (length) )
#define txtcmp( txt1, txt2 ) Text_compare( (txt1), (txt2) )


typedef struct
Text
{
    const char   *data;   /* ALWAYS first */
    const size_t  length; /* ALWAYS second */
}
Text;
typedef Sequence    TextBuffer;


char    Text_charAt(   Text txt,  size_t index);
int     Text_compare(  Text txt1, Text   txt2);
int     Text_find(     Text txt,  Text   needle);
int     Text_findChar( Text txt,  char   c);
size_t  Text_length(   Text txt);
int     Text_rfind(    Text txt,  Text   needle);
char   *Text_toCStr(   Text txt);


/* Constructor / Destructor */
TextBuffer TextBuffer_new(     size_t      capacity);
TextBuffer TextBuffer_fromText(Text        txt);
TextBuffer TextBuffer_fromCStr(const char *cstr);
void       TextBuffer_free(    TextBuffer  buf);


/* 
	TextBuffer Methods 
*/
/* Access */
size_t     TextBuffer_length(   TextBuffer buf);
/* Mutators */
void       TextBuffer_clear(    TextBuffer buf);
void       TextBuffer_concat(   TextBuffer buf, Text   txt);
void       TextBuffer_nConcat(  TextBuffer buf, Text   txt,   size_t length);
void       TextBuffer_insert(   TextBuffer buf, size_t index, Text   txt);
void       TextBuffer_setChar(  TextBuffer buf, char   c,     size_t index);
TextBuffer TextBuffer_substring(TextBuffer buf, size_t start, size_t length);


#endif /* BTCHWRK_TEXT_H */
