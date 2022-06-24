/* Build Render Tree*/

#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "lexerParser.h"

#define BOLD 1
#define ITALIC 2
#define UNDERLINE 4
#define DELETED 8
#define HYPER 16
#define HR 32
#define SMASHED 64

#define MAX_RENDER_CHILDS 128
#define MAX_LINE_OBJS 128
#define MAX_HLINK 128

#define SAVEPEN(x)  int x##style__=GetStyle(),x##point__=GetPointSize()\
,x##pen__=GetPenSize();string x##color__=GetPenColor();

#define RECOVERPEN(x)  SetStyle(x##style__);SetPointSize(x##point__);\
SetPenColor(x##color__);SetPenSize(x##pen__);

#define RN(x) ((x)==NULL?"":(x))
#define getAttr(x,y) ((x)->named_attributes[hash(y)])
#define GetFontWidth() TextStringWidth("_")

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

typedef struct PRINT_OPT {
	byte states;//bool flags
	uint_32 color,size;
}PRINT_OPT;

typedef struct RENDER {
	struct RENDER *childs[MAX_RENDER_CHILDS];
	float_64 w,h,align_h;
	float_64 x,y;
	uint_32 child_count;
	string print_text;
	string tagtype;
	string id;
	PRINT_OPT opt;
}RENDER;

extern float_64 shift_x,shift_y;
extern bool lazying; 
extern string globalCSS;
extern string title;
extern string redirect;
void mainRender(float_64 x,float_64 y,float_64 w,float_64 h,RENDER *root);
void drawHtml(float_64 x,float_64 y,float_64 w,float_64 h);
void applyPrintOpt(PRINT_OPT opt);
PRINT_OPT updateOpt(DOM *root, PRINT_OPT opt);
RENDER* buildRenderTree(DOM *root,PRINT_OPT opt);
RENDER *findRenderByID(string id, RENDER *node);
void jumpByID(string id);

static inline uint_32 str2color(string s) //字符串哈希函数 
{
	uint_32 color=0;
	if (s[0]=='#') s++; // 必须符合以下格式(正则)：#[0-9a-fA-F]{6}
	sscanf(s, "%x", &color);
	return color;
}
#endif
