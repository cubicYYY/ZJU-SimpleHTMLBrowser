#ifndef LEXER_PAESER_H
#define LEXER_PAESER_H

#define HTML_BUF_SIZE 65536
#define MAX_DOM_CHILDS 127
#define MAX_NODES 2048
#define MAX_TOKENS 16384
#define MAX_ATTRS 16
#define MAX_NAME_L 16
#define HASH_SIZE 31
#define HASH_CONSTANT 29
#define MAX_STR_LENGTH 512
#define MAX_COLOR_LENGTH 10

#include "strlib.h"
#include "types.h"
typedef enum{
	NONE,
	MISMATCHED_TAG,
	UNCLOSED_TAG,
	TOO_MANY_CHILDS,
	TOO_MANY_NODES,
	TOO_MANY_ATTRS,
	
	ILLIGALEX_DATA_TYPE
}P_ERROR;
typedef enum{
	INTEGER,
	STRING
}DATA_TYPE;
typedef enum{
	LEX_EQ,
	LEX_SPA,
	LEX_TAG_SL,//	< 
	LEX_TAG_SR,//	>
	LEX_TAG_CR,//	/>
	LEX_TAG_CL,//	</
	LEX_INTNUM,
	LEX_QUOTE,
	LEX_STRING,
	LEX_EOT
}Lexical;
typedef union DATA{
	int_64 integer;
	string str_p;
}DATA;
typedef struct TOKEN {
	Lexical type;
	uint_32 line,col;
	DATA_TYPE data_type;
	DATA data;
}TOKEN;
typedef struct DOM {
	string named_attributes[HASH_SIZE];
    struct DOM* childs[MAX_DOM_CHILDS]; //vector is better but no STL use is allowed
    uint_32 child_count;
    string tagtype;
    string literal; //to string
}DOM;


extern string html_str;
extern P_ERROR err;
extern float_64 rend_x,rend_y;
extern float_64 area_w,area_h;

TOKEN* htmlLexer(string s);
TOKEN * tagParser(TOKEN *s, DOM *father);
DOM* htmlParser(string s);

static inline uint_32 hash(string s)
{
	uint_32 hash_v=0;
	while (*s!='\0')
		hash_v=hash_v*HASH_CONSTANT+*(s++);
	return hash_v%HASH_SIZE;
}
#endif 
