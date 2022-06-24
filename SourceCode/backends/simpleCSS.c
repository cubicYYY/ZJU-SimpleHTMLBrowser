#include <stdio.h>
#include "simpleCSS.h"
#define CSS_HASH_SIZE 1023

string noSpace(string str)
{
	string newstr=CopyString(str);
	char *i=str,*j=newstr;
	while (*i!='\0') {
		if (*i==' '||*i=='\n'||*i=='\r'||*i=='\t') {
			i++;
			continue;
		}
		*j=*i;
		j++,i++;
	}
	*j='\0';
	return newstr;
}
string extractSubAttr(string node_attr,string subattr) 
{
	int_32 find_pos;//! Must be signed, since result can be -1 
	if (node_attr && (find_pos=FindString(Concat(subattr, ":"), node_attr, 0))!=-1) {
		uint_32 start_pos=find_pos+StringLength(subattr)+1;
		uint_32 end_pos=FindString( ";", node_attr, start_pos);
		return ConvertToLowerCase(SubString(node_attr, start_pos, end_pos-1));
	}
	return NULL;
}

PRINT_OPT updateOptByCSSText(PRINT_OPT opt, string style) //通过CSS文本更新打印渲染选项 
{
	style = noSpace(style);
	style=Concat(style,";");
	string strtmp;
	if ((strtmp=extractSubAttr(style,"font-size"))) 
		opt.size = StringToInteger(strtmp);
	if (StringEqual(RN(extractSubAttr(style,"font-weight")), "bold")) 
		opt.states|=BOLD;
	if (StringEqual(RN(extractSubAttr(style,"font-style")), "italic")) 
		opt.states|=ITALIC;
	if (StringEqual(RN(extractSubAttr(style,"font-decoration")), "underline")) 
		opt.states|=UNDERLINE;
	if (StringEqual(RN(extractSubAttr(style,"font-decoration")), "line-through")) 
		opt.states|=DELETED;
	if (StringEqual(RN(extractSubAttr(style,"display")), "none")) 
		opt.states|=SMASHED;
	if ((strtmp=extractSubAttr(style,"color"))) 
		opt.color = str2color(strtmp);
	return opt;
}

PRINT_OPT updateOptByID(PRINT_OPT opt, string fullCSS, string id)
{
	fullCSS = noSpace(fullCSS);
	id = noSpace(id);
	int_32 start_pos=FindString(Concat(Concat("#",id),"{"), fullCSS, 0);
	if (start_pos==-1) return opt;
	int_32 end_pos=FindString("}", fullCSS, start_pos);
	string CSSText = SubString(fullCSS, start_pos+StringLength(id)+2, end_pos-1);
	return updateOptByCSSText(opt, CSSText);
}

PRINT_OPT updateOptByClass(PRINT_OPT opt, string fullCSS, string classes)
{
	fullCSS = noSpace(fullCSS);
	classes=Concat(classes," ");
	int_32 last_space=-1,string_pos=0;
	while ((string_pos=FindString(" ",classes,last_space+1))!=-1) {
		string now_class=SubString(classes,last_space+1,string_pos-1);
		last_space=string_pos;
		now_class = noSpace(now_class);
		int_32 start_pos=FindString(Concat(Concat(".",now_class),"{"), fullCSS, 0);
		if (start_pos==-1) 
			continue;
		int_32 end_pos=FindString("}", fullCSS, start_pos);
		string CSSText = SubString(fullCSS, start_pos+StringLength(now_class)+2, end_pos-1);
		puts(CSSText); 
		opt=updateOptByCSSText(opt, CSSText);
	}
	return opt;
}

