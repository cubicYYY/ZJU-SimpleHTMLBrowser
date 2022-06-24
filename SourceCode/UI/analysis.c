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
#include "analysis.h"
#include "renderEngine.h"
#include "simpleCSS.h"
extern bool lazying;
extern string html_str;
extern Color_ c;
string wait_for_jump=NULL;
void masking()//提供遮罩 防止溢出 
{
	double x = GetWindowWidth();
	double y = GetWindowHeight();
	double h = WinHeight/10;
	SetPenColor(c.down);
	drawRectangle(0, 0,x,y/15, 1);
	drawRectangle(0, y/14+y/1.5,x,y/10, 1);
	drawRectangle(0, 0, x/26,y-y/6, 1);
	drawRectangle(x-x/25, 0, x/25,y-y/6, 1);
	if (style) {
		drawRectangle(x/25+x/2.4, 0, x-x/25-x/2.4-x/25-x/2.4,y-y/6, 1);
	}
}
void analysis()//解析渲染控制器 
{
	double fH=0.027816*WinHeight;
	double h = fH*2;
	double x = WinWidth/18;
	double y = WinHeight-h*5+shift_y;
	char temp[1024];
	fp = fopen(textbuf2[tabNumber], "rb");
	if( fp == NULL) {

		newflag = 0;
		if(textbuf2[tabNumber][0]==0) {
			SAVEPEN(init_);
			SetPointSize(20);
			y-=shift_y;
			MovePen(x,y-h);
			DrawTextString("键入index.html以开始");
			y+=shift_y;
			RECOVERPEN(init_);
		} else { //404
			y-=h;
			y-=shift_y;
			MovePen(x,y);
			SetPointSize(50);
			DrawTextString("404");
			SetPointSize(23);
			MovePen(x,y-0.3);
			DrawTextString("Ooooops!");
			MovePen(x,y-0.6);
			DrawTextString("哪儿呢QAQ");
			MovePen(x,y-0.9);
			DrawTextString("喵喵喵");
			y+=h;
			y+=shift_y;
		}
	} else {
		if (style) {
			SetPointSize(14);
			while(fgets(temp,1023,fp)) {
				MovePen(x,y-=fH*1.2);
				if (y<WinHeight*0.1)
					break;
				if (y<WinHeight*0.7)
					DrawTextString(temp);
			}
			if(pageFlash==1) {
				newflag = 1;
				lazying=FALSE;
			}
		}
		rewind(fp);
		html_str=(string)GetBlock((HTML_BUF_SIZE+1)*sizeof(string));
		fread(html_str,HTML_BUF_SIZE,sizeof(char),fp);
		double minor=WinHeight*0.02;
		if (style)
			drawHtml(WinWidth-WinWidth/25-WinWidth/2.4,WinHeight/15+WinHeight/1.5-minor,WinWidth/2.4,WinHeight/1.5-minor);
		else
			drawHtml(WinWidth/25,WinHeight/15+WinHeight/1.5-minor,WinWidth/2.4*2,WinHeight/1.5-minor);
		masking();
		if (redirect && StringLength(redirect)>1) { //需要重定向 
			puts(redirect);
			shift_x=shift_y=0;
			int_32 shrp_pos=FindChar('#',redirect,0);//分离url与id锚点定位
			if (shrp_pos==0) {
				jumpByID(SubString(redirect,1,StringLength(redirect)));
				redirect=NULL;
				pageFlash=1;
			}	
			else {
				if (shrp_pos==-1) shrp_pos=StringLength(redirect)+1;
				strcpy(textbuf2[tabNumber],SubString(redirect,0,shrp_pos-1));
				wait_for_jump = noSpace(SubString(redirect,shrp_pos+1,StringLength(redirect)));
				jumpByID(wait_for_jump);
				puts(wait_for_jump);
				lazying=FALSE;
				pageFlash=1;
				fclose(fp);
				redirect=NULL;
				return;
			}
			
		}

	}
	pageFlash = 0;
	fclose(fp);


}

