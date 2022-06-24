#include <strlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "color.h"
#include "tabs.h"
#include "renderEngine.h"
int tabNumber;//第几个Tab
int totaltabNumber;	//此时共几个Tab
int pageFlash; //是否解析文件的判断
int newflag; //是否简历链表新节点的判断
int Tabflag;//标签页的判断
char textbuf2[MAX][MAX];//全局变量存放网址（文件名）
char add[MAX];
FILE *fp;

// 输入框输入相关网址（文件名）
void addinput()
{
	double fH =0.027816*WinHeight;
	double h = fH*2;
	double x = WinWidth/3;
	double y = WinHeight-h/12;
	double w = WinWidth/2;
	SetPenColor(c.letter);
	SetPointSize(18);
	drawLabel(x-4*fH-TextStringWidth("请输入网址:"), (y-h*1.5)+fH*0.7, "请输入网址:");
	textbox(GenUIID(0), x-0.05*WinWidth, y-1.45*h, w, h, add, sizeof(add));
}

//标签页
void drawTabs()
{
	double h = WinHeight/10;
	double x = 0;
	double y = WinHeight-h;
	char Tab[MAX];
	int i;

	SetPointSize(18);
	setButtonColors(c.lable, c.letter,c.lable, c.letter, 1);
	for(i=1; i<=totaltabNumber; i++) { //用i循环画Tab
		fp = fopen(textbuf2[i], "r");
		if(fp!=NULL) {
			if (!title || StringEqual(title,""))
				strcpy(Tab,textbuf2[i]);
			else strcpy(Tab, title);
		} else if(fp==NULL) {
			if(textbuf2[i][0]==0) {
				strcpy(Tab,"blank"); //空标签页
			} else {
				strcpy(Tab,"404");  //错误网址
			}
		}
		fclose(fp);
		if(button(GenUIID(i), x+(WinWidth/6)*(i-1), y-h/1.7,WinWidth/6.5,h/1.7,Tab)) {
			tabNumber = i;
			pageFlash = 1;

		}
		if(button(GenUIID(i), (WinWidth/6)*i-0.6/16*WinWidth, y-h/2+0.08/16*WinHeight,0.3/16*WinWidth,h/3,"×")) { //关闭标签页
			int k=0;
			for(; i+k<totaltabNumber; k++) {
				t1 = head[i+k];
				t2 = head[i+k+1];
				printf("0  ");

				while(t2!=NULL&&t1!=NULL) {

					strcpy(t1->address,t2->address);
					t2 = t2->next;
					t1 = t1->next;

				}
				printf("1  ");
				if(t1!=NULL&&t2==NULL) {
					if(head[i+k+1]!=NULL) {
						//printf("2  ");
						tail[i+k] = t1->pre;
						while(t1!=NULL) {
							p = t1;
							t1 = t1->next;
							free(p);
						}
					} else if(head[i+k+1]==NULL) {
						//printf("3  ");

						t1 = t1->next;
						while(t1!=NULL) {
							p = t1;
							t1 = t1->next;
							free(p);
						}
						tail[i+k] = NULL;
						head[i+k] = NULL;
					}
				}
				if(t1==NULL&&t2!=NULL) {

					if(head[i+k]!=NULL) {

						while(t2!=NULL) {

							p = (Addlink)malloc(sizeof(struct Address));
							strcpy(p->address,t2->address);
							tail[i+k]->next = p;

							p->pre = tail[i+k];
							p->next = NULL;
							tail[i+k] = tail[i+k]->next;
							t2 = t2->next;
							if(t2!=NULL) printf("%s",tail[i+k]->address);
						}
					}
					printf("4  ");
					if(head[i+k]==NULL) {
						//	printf("5  ");
						while(t2!=NULL) {

							p = (Addlink)malloc(sizeof(struct Address));
							if (head[i+k] == NULL) {
								//printf("6  ");
								head[i+k] = p;
								head[i+k]->pre = NULL;
								tail[i+k] = p;
								p->next = NULL;
								strcpy(p->address,t2->address);
								t2=t2->next;
							}

							else if (tail[i+k]->next==NULL) {
								//printf("7  ");
								tail[i+k]->next = p;
								p->pre = tail[i+k];
								tail[i+k] = p;
								strcpy(p->address,t2->address);
								p->next = NULL;
								t2 = t2->next;
							}
						}
					}
				}

				strcpy(textbuf2[i+k],textbuf2[i+k+1]);
			}

			strcpy(textbuf2[i+k],"");
			printf("  %d",i+k);
			if(head[i+k]!=NULL) {

				t = head[i+k]->next;

				while(t!=NULL) {
					p = t;
					t = t->next;
					free(p);
				}
				head[i+k] = NULL;
				tail[i+k] = NULL;
			}
			//	printf("8  ");
			if(i==totaltabNumber) {
				tabNumber = i-1;
			} else if(i!=totaltabNumber) {
				tabNumber = i;
			}
			totaltabNumber--;
			pageFlash=1;
		}
		if(i==totaltabNumber) {
			SetPointSize(18);
			if(button(GenUIID(i), (WinWidth/6)*i+h*0.03, y-h*0.45,h/3,h/3,"+")) { //新增标签页
				tabNumber++;

				totaltabNumber++;
				pageFlash=1;
			}
		}
	}
	if(totaltabNumber==0) {
		SetPointSize(18);
		if(button(GenUIID(0), (WinWidth/6)*0, y-h/2.5,0.3/16*WinHeight,h/3,"+")) { //新增标签页
			totaltabNumber++;
			pageFlash=1;
		}
	}
	SetPenSize(2);
	SetPenColor(c1.letter);
	MovePen(0,y);
	DrawLine(WinWidth,0);
	MovePen(0,y-0.6*h);
	DrawLine(WinWidth,0);  //标签页边框
}


// 新建链表节点函数
void AddAddtoLink()
{
	p = (Addlink)malloc(sizeof(struct Address));
	if (head[tabNumber] == NULL) {
		head[tabNumber] = p;
		head[tabNumber]->pre = NULL;
		tail[tabNumber] = p;
		p->next = NULL;
		p->pre = head[tabNumber];
		strcpy(p->address,textbuf2[tabNumber]);
	}

	else if (tail[tabNumber]->next==NULL) {
		tail[tabNumber]->next = p;
		p->pre = tail[tabNumber];
		tail[tabNumber] = p;
		strcpy(p->address,textbuf2[tabNumber]);
		p->next = NULL;
	}
	newflag = 0;
}
