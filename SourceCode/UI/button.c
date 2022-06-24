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
int style=1; //���ڵ���ʽ������1 �ϲ�0

//���Ʒ�����ť
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
	//������ɫ���ڰ�ť
	setButtonColors(c.button, "White",c.button,"White", 1);
	if (button(GenUIID(0),WinWidth/1.25,WinHeight/1.07,WinWidth/17,WinHeight/22, color ? "sun" : "sea"))
		color = ! color;

}



// ���װ�ť��������ǰ�����������ˡ�����ˢ�¡�
void Button()
{
	double h = WinHeight/11;
	double x = 0;
	double y = WinHeight-h;
	double w = WinWidth/20;
	SetPointSize(10);

	// ������˼�����Ӧ
	setButtonColors(c.button, c.button,c.button, c.button, 1);
	if(button(GenUIID(0), x+0.07/16*WinWidth, y+0.15/16*WinHeight, 0.8*w, 0.8*h, "<" )) {
		if(head[tabNumber]!=NULL&&tail[tabNumber]->pre!=NULL) {
			tail[tabNumber]=tail[tabNumber]->pre;
			strcpy(textbuf2[tabNumber],tail[tabNumber]->address);
			pageFlash=2; //��ǰ�����˼���pageFlash=2ֻ���ʣ������½������� ;��������س���pageFlash=1�����½�����ڵ�
		} else if(head[tabNumber]!=NULL&&tail[tabNumber]->pre==NULL) {
			DisplayClear();
			Button();
			addinput();
		}
		lazying=FALSE;
	}
	// ���ˢ�¼�����Ӧ
	if(button(GenUIID(0), x+1.1*w+0.07/16*WinWidth, y+0.15/16*WinHeight,0.8*w, 0.8*h, "O" )) {
		if(head[tabNumber]!=NULL) pageFlash=2;
		lazying=FALSE;
		shift_x=shift_y=0;
	}
	// ���ǰ��������Ӧ
	if(button(GenUIID(0), x+2.2*w+0.07/16*WinWidth, y+0.15/16*WinHeight, 0.8*w, 0.8*h, ">" )) {
		if(head[tabNumber]!=NULL&&tail[tabNumber]->next!=NULL) {
			tail[tabNumber]=tail[tabNumber]->next;
			strcpy(textbuf2[tabNumber],tail[tabNumber]->address);
			pageFlash=2;
		}
		lazying=FALSE;
	}
	//��������ťͼ��
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

