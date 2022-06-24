#ifndef SIMPLE_CSS_H
#define SIMPLE_CSS_H

#include "renderEngine.h"
string extractSubAttr(string node_attr,string subattr);
PRINT_OPT updateOptByCSSText(PRINT_OPT opt, string style);
PRINT_OPT updateOptByID(PRINT_OPT opt, string fullCSS, string id);
PRINT_OPT updateOptByClass(PRINT_OPT opt, string fullCSS, string classes);
string noSpace(string str);
 
#endif
