#ifndef TABS_H
#define TABS_H

#define MAX 1000
extern int tabNumber;//�ڼ���Tab
extern int totaltabNumber;	//��ʱ������Tab
extern int pageFlash; //�Ƿ�����ļ����ж�
extern int newflag; //�Ƿ���������½ڵ���ж�
extern int Tabflag;//��ǩҳ���ж�
extern char textbuf2[MAX][MAX];//ȫ�ֱ��������ַ���ļ�����
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
