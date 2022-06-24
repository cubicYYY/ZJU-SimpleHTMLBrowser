/*主HTML词法语法解析器*/
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexerParser.h"

static string singleTags="meta|link|br|hr|img|area|input|basefont|param|col|frame|embed|keygen|source|base|!DOCTYPE|";
static size_t token_count = 0;//总token个数 
static size_t DOM_node_count = 0;//总DOM节点数 
static TOKEN tok_stream[MAX_TOKENS];
static DOM nodes[MAX_NODES];
string html_str;//!全局变量+extern：传入的html字符串 
P_ERROR err;
static void dout(TOKEN *o)//for debug only
{
#ifdef DEBUG__
	string tmp_name[]={"LEX_EQ","LEX_SPA","LEX_TAG_SL","LEX_TAG_SR","LEX_TAG_CR","LEX_TAG_CL","LEX_INTNUM","LEX_QUOTE","LEX_STRING","LEX_EOT"};
    printf("TOKEN{type=%s,",tmp_name[o->type]);
    if (o->data_type==INTEGER) printf("i:%d",(o->data.integer));
    else if (o->data_type==STRING)  printf("s:%s",o->data.str_p);
    puts("}");
#endif
}


static inline void initLexerParser()//初始化词法文法解析器 
{
	token_count = 0;
	DOM_node_count = 0;
	err = NONE;
}

static inline void addAttr(DOM *node, const string attr_name, string attr_content)//为DOM节点添加属性 
{
	node->named_attributes[hash(attr_name)] = CopyString(Concat(attr_content, " "));
	//extra space for better matching
	//利用哈希表 
}
static void addToken(Lexical type, uint_32 line, uint_32 col, DATA_TYPE datatype, DATA data)//添加token 
{
	tok_stream[token_count].line = line;
	tok_stream[token_count].col = col;
	tok_stream[token_count].type = type;
	tok_stream[token_count].data_type = datatype;
	tok_stream[token_count].data = data;
	dout(&tok_stream[token_count]);
	token_count++;
}

TOKEN* htmlLexer(string s)//将纯文本HTML字符串转为token流 
{ 
    int row = 1, col = 1;
    DATA data;
    DATA_TYPE datatype=STRING;
    while (*s!='\0') {
    	datatype=STRING;
        Lexical type=LEX_STRING;
        data.integer=0;
        uint_32 inc = 1;
		
		char now=*s;
        if (now==' '||now=='\t') {
        	data.str_p=CopyString(" ");
        	addToken(LEX_SPA, row, col, datatype, data);
			while (*s==' '||*s=='\t') {
				s++;
        		col++;
			}
    		
            continue;
        } 
        else if (now=='\r') {
            s++;
            continue;
        }
        else if (now=='\n') {
            row++;
            col=1;
            s++;
            continue;
        }

        bool noAdd=FALSE;
        switch (now) {
            case '>':
            	data.str_p = CharToString('>');
                type = LEX_TAG_SR;
                break;
            case '=':
            	data.str_p = CharToString('=');
                type = LEX_EQ;
                break;
            case '"':
            	data.str_p = CharToString('"');
                type = LEX_QUOTE;
                break;
            case '/':
            	if (*(s+1)=='>') {
            		data.str_p = CopyString("/>");
	                type = LEX_TAG_CR;
	                inc=2;
            	} else 
            		data.str_p = CharToString('/');
                break;
            case '<':
                if (*(s+1)=='/') {
                    type = LEX_TAG_CL;
                    data.str_p=CopyString("</");
                    inc = 2;
                }
                else if (*(s+1)=='!') {
                    inc=0;
                    while(*(s+inc)!='\0'&&*(s+inc)!='>') 
                        inc++;
                    inc++;
                    noAdd=TRUE;
                    break;
                } else {
                	data.str_p = CharToString('<');
	            	type = LEX_TAG_SL;
                }
                    
                break;
			case '&'://转义字符 
				type = LEX_STRING;
				if (FindString("&amp;", s, 0)==0)
					data.str_p = CopyString("&"),inc=5;
				else if (FindString("&nbsp;", s, 0)==0)
					data.str_p = CopyString(" "),inc=6;
				else if (FindString("&lt;", s, 0)==0)
					data.str_p = CopyString("<"),inc=4;
				else if (FindString("&gt;", s, 0)==0)
					data.str_p = CopyString(">"),inc=4;
				else if (FindString("&quot;", s, 0)==0)
					data.str_p = CopyString("\""),inc=6;
				break;    	
            default:
                if (isdigit(now)) {
                    //cout<<"!";
                    type = LEX_INTNUM;
                    inc=0;
                    int_64 num=0;
                    while(isdigit(*(s+inc))) {
                        num=num*10+*(s+inc)-'0';
                        inc++;
                    }
                    data.integer=num;
                    datatype = INTEGER;
                } 
                else {
                    inc=0;
                    while(*(s+inc)!='\0' && *(s+inc)!='\r' && *(s+inc)!='\n' && *(s+inc)!='/' &&
						*(s+inc)!='<' && *(s+inc)!=' ' && *(s+inc)!='\0' && *(s+inc)!='>' && *(s+inc)!='=' && *(s+inc)!='"' && *(s+inc)!='&' ) 
							 inc++;     
                    string tmp=(string)GetBlock((inc+1)*sizeof(char));
                    memcpy(tmp, s, inc*sizeof(char));
                    *(tmp+inc) = '\0';
                    
                    type = LEX_STRING;
                    data.str_p = tmp;
                    datatype = STRING;
                } 
                break;
        }
        if (!noAdd) addToken(type, row, col, datatype, data);
        s+=inc;
        col+=inc;
    }
    addToken(LEX_EOT, row, col, INTEGER, data);
    return tok_stream;
}
static DOM* addDOMNode(const string tagtype,const string literal,DOM* father)//添加DOM节点 
{
	printf("type:%s lit:%s fat:%s\n",tagtype,literal,father==NULL?"":father->tagtype);
    if (DOM_node_count==MAX_NODES) {
        err = TOO_MANY_NODES;
        return NULL;
    }
    DOM *node = &nodes[DOM_node_count];
	node->child_count=0;
//    node->father=father;
    if (father!=NULL && node!=NULL) {
        father->childs[father->child_count]=node;
        father->child_count++;
    }
    node->literal=(string)literal;
    node->tagtype=ConvertToLowerCase((string)tagtype);
    memset(node->named_attributes,0,sizeof(node->named_attributes));
    ++DOM_node_count;
    return node;
} 
TOKEN * tagParser(TOKEN *s, DOM *father)//对token流进行解析 
{
	dout(s);
    if (father==NULL)
        father = addDOMNode("none", "", NULL);
    DOM *current=NULL;
    bool named = FALSE;
    string name_p=NULL, attr_name=NULL;
    while (s->type != LEX_EOT && s->type != LEX_TAG_SL) {
    	addDOMNode("text", (s->data_type==STRING ? s->data.str_p : IntegerToString(s->data.integer)), father);
    	s++;
    }
    if (s->type==LEX_EOT) return NULL;
    s++;
    
    /*******HEAD标签头部************/
    while (s->type != LEX_EOT && s->type != LEX_TAG_SR && s->type != LEX_TAG_CR) {
   		dout(s);
        if (!named) { //name
            if (s->type != LEX_STRING) {
                err = ILLIGALEX_DATA_TYPE;
                s++;
                continue;
            } 
            name_p = s->data.str_p;
            named = TRUE;
            current = addDOMNode(name_p, "", father);
        } 
        else if (s->type == LEX_STRING) { //attribute
        	attr_name = (s->data_type==STRING ? ConvertToLowerCase(s->data.str_p) : IntegerToString(s->data.integer));
            s++;
            if (s->type != LEX_EQ) {
                s++;
                continue;
            } 
            s++;
            if (s->type == LEX_STRING)
            	addAttr(current, attr_name, s->data.str_p);
            else if (s->type == LEX_QUOTE) {
            	s++;
            	string attr_content=GetBlock((MAX_STR_LENGTH+1)*sizeof(char));
            	memset(attr_content,0,sizeof(attr_content));
            	int shift=0;
            	while (s->type != LEX_QUOTE) {
            		if (s->data.str_p==NULL) {
            			s++;
            			continue;
            		}
            		string str=(s->data_type==STRING ? s->data.str_p : IntegerToString(s->data.integer));
            		int lenlen=strlen(str);
            		memcpy(attr_content+shift,str,lenlen*sizeof(char));
            		shift+=lenlen;
            		attr_content[shift]=' ';
            		shift++;
            		s++;
            	}
            	*(attr_content+shift)='\0';
            	s++;
            	addAttr(current, attr_name, attr_content);
            	continue;
            }
        }
        s++;
    }
    //单标签已经可以结束本个标签的解析 
    if (s->type==LEX_EOT) return NULL;
	if (s->type==LEX_TAG_CR||FindString(Concat(singleTags,"|"), name_p, 0)!=-1)
		return ++s;
	s++;
	while (s->type==LEX_SPA) s++;
	
	
	/*******BODY标签内部************/
    while (s->type != LEX_EOT && s->type!=LEX_TAG_CL) { 
    	if (s->type==LEX_SPA) {
    		addDOMNode("text", " ", current);
    		while(s->type==LEX_SPA) s++;
    		continue;
    	}
    	if (s->type==LEX_TAG_SL) {
    		TOKEN* targ=tagParser(s, current);
        	s = targ; 
    	} else {
    		addDOMNode("text", (s->data_type==STRING ? s->data.str_p : IntegerToString(s->data.integer)), current);
    		s++;
    	}
    }
    if (s->type==LEX_EOT) return NULL;
    
    /*******TAIL结束标签************/
    while(s->type!=LEX_TAG_SR && s->type!=LEX_EOT)
    	s++;
    if (s->type!=LEX_EOT) 
		return ++s;
	else
		return NULL;
}
DOM* htmlParser(string s)
{
	
	initLexerParser();
	DOM *root = addDOMNode("root", "", NULL);//根节点 
	TOKEN *tokIter = htmlLexer(s);
	puts("--------");
	while((tokIter=tagParser(tokIter, root)));
	return root;
}

