#ifndef TABS_H
#define TABS_H

#define MAX 1000
extern int tabNumber;//第几个Tab
extern int totaltabNumber;	//此时共几个Tab
extern int pageFlash; //是否解析文件的判断
extern int newflag; //是否简历链表新节点的判断
extern int Tabflag;//标签页的判断
extern char textbuf2[MAX][MAX];//全局变量存放网址（文件名）
extern char add[MAX]; 
extern FILE *fp;

typedef struct Address* Addlink;
struct Address {
	Addlink pre;
	char address[MAX];
	Addlink next;
}*head[7],*p,*tail[7],*t1,*t2,*t;

void addinput();
void drawTabs();
void AddAddtoLink();

#endif
