#include <strlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "color.h"
#include "button.h"
#include "tabs.h"
#include "renderEngine.h"
extern bool lazying;
int style=1; //窗口的形式：分屏1 合并0

//绘制分屏按钮
void DrawStyleButtons()
{
	setButtonColors(c.button, "White",c.button, "White", 1);
	SetPointSize(18);
	if (button(GenUIID(0),WinWidth/1.15,WinHeight/1.07,WinWidth/10,WinHeight/22, style ? "combine" : "divide")) {
		style = ! style;
		lazying=FALSE;
	}
		

	if( style ) {
		SetPenColor(c.letter);
		MovePen(WinWidth/2,0.733*WinHeight);
		DrawLine(0,-WinHeight*9.3/14);
		drawLabel(WinWidth/5, WinHeight/1.3, "HTML Code");
		drawLabel(WinWidth/1.5, WinHeight/1.3, "Result");
	} else {
		SetPenColor(c.text);
//		drawRectangle(WinWidth/25, WinHeight/15,WinWidth/1.5,WinHeight/1.5, 1);
	}
	//补画颜色调节按钮
	setButtonColors(c.button, "White",c.button,"White", 1);
	if (button(GenUIID(0),WinWidth/1.25,WinHeight/1.07,WinWidth/17,WinHeight/22, color ? "sun" : "sea"))
		color = ! color;

}



// 简易按钮，包含“前进”、“后退”、“刷新”
void Button()
{
	double h = WinHeight/11;
	double x = 0;
	double y = WinHeight-h;
	double w = WinWidth/20;
	SetPointSize(10);

	// 点击后退键的响应
	setButtonColors(c.button, c.button,c.button, c.button, 1);
	if(button(GenUIID(0), x+0.07/16*WinWidth, y+0.15/16*WinHeight, 0.8*w, 0.8*h, "<" )) {
		if(head[tabNumber]!=NULL&&tail[tabNumber]->pre!=NULL) {
			tail[tabNumber]=tail[tabNumber]->pre;
			strcpy(textbuf2[tabNumber],tail[tabNumber]->address);
			pageFlash=2; //按前进后退键，pageFlash=2只访问，不会新建链表结点 ;而如果按回车，pageFlash=1，会新建链表节点
		} else if(head[tabNumber]!=NULL&&tail[tabNumber]->pre==NULL) {
			DisplayClear();
			Button();
			addinput();
		}
		lazying=FALSE;
	}
	// 点击刷新键的响应
	if(button(GenUIID(0), x+1.1*w+0.07/16*WinWidth, y+0.15/16*WinHeight,0.8*w, 0.8*h, "O" )) {
		if(head[tabNumber]!=NULL) pageFlash=2;
		lazying=FALSE;
		shift_x=shift_y=0;
	}
	// 点击前进键的响应
	if(button(GenUIID(0), x+2.2*w+0.07/16*WinWidth, y+0.15/16*WinHeight, 0.8*w, 0.8*h, ">" )) {
		if(head[tabNumber]!=NULL&&tail[tabNumber]->next!=NULL) {
			tail[tabNumber]=tail[tabNumber]->next;
			strcpy(textbuf2[tabNumber],tail[tabNumber]->address);
			pageFlash=2;
		}
		lazying=FALSE;
	}
	//画三个按钮图标
	SetPenSize(5);
	SetPenColor(c1.down);
	MovePen(x+w/2+h/3,y+h/2);
	DrawArc(h/3,0,360);
	MovePen(x+w*2/3,y+h/2);
	DrawLine(-w/3,0);
	DrawLine(w/8,h/8);
	MovePen(x+w/3,y+h/2);
	DrawLine(w/8,-h/8);

	MovePen(w*2+x+w/1.5+h/3,y+h/2);
	DrawArc(h/3,0,360);
	MovePen(w*2+x+w/2,y+h/2);
	DrawLine(w/3,0);
	DrawLine(-w/8,h/8);
	MovePen(w*2+x+w*5/6,y+h/2);
	DrawLine(-w/8,-h/8);

	MovePen(w+x+w/1.5-0.04/16*WinWidth,y+h/4-0.07/16*WinHeight);
	DrawArc(h/3.4,-90,270);
	DrawLine(-w/17,h/6);
	MovePen(w+x+w/1.5-h/3.4-0.04/16*WinWidth,y+h/3.4+h/4-0.07/16*WinHeight);
	DrawLine(w/6,h/10);
	SetPenSize(1);

}

