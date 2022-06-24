#include <strlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "color.h"


int color=1;//主页面颜色：blue or yellow

extern int style; 

//自定义颜色
void Color()
{
	//第一套蓝绿配色
	DefineColor("1-up", 0.737, 0.796, 0.871);
	DefineColor("1-mid", 0.761, 0.867, 0.902);
	DefineColor("1-down",0.957, 0.955, 0.977);
	DefineColor("1-text", 0.902, 0.914, 0.941);
	DefineColor("1-letter", 0.08, 0.373, 0.678);
	DefineColor("1-button", 0.635, 0.698, 0.898);
	DefineColor("1-lable", 0.65, 0.8, 0.85);

	//第二套红黄配色
	DefineColor("2-up", 0.985,0.776,0.58);
	DefineColor("2-mid",0.988,0.871,0.718);
	DefineColor("2-down", 0.992,0.969,0.875);
	DefineColor("2-text", 0.98,0.95,0.8);
	DefineColor("2-letter", 0.537, 0.639, 0.773);
	DefineColor("2-button",0.986,0.706,0.6);
	DefineColor("2-lable",1,0.82,0.6);
}


//改变页面色系
void colorstyle()
{
	setButtonColors(c1.button, "White",c1.button,"White", 1);
	SetPointSize(18);
	if (button(GenUIID(0),WinWidth/1.25,WinHeight/1.07,WinWidth/17,WinHeight/22, color ? "yellow" : "blue"))
		color = ! color;

	if( color ) {
		c=c1;
	} else {
		c=c2;
	}


}

//页面颜色填充
void fillcolors(char* BackgroundColor,char* TextColor,char*UpColor,char*MiddleColor)
{

	double x = GetWindowWidth();
	double y = GetWindowHeight();
	double h = WinHeight/10;

	SetPenColor(BackgroundColor);
	drawRectangle(0, 0, x, y, 1);
	SetPenColor(TextColor);
	drawRectangle(WinWidth/25, WinHeight/15,WinWidth/2.4,WinHeight/1.5, 1);
	drawRectangle(x-WinWidth/25-WinWidth/2.4, WinHeight/15,WinWidth/2.4,WinHeight/1.5, 1);
	SetPenColor(UpColor);
	drawRectangle(0, WinHeight-WinHeight/10,WinWidth,WinHeight/10, 1);
	SetPenColor(MiddleColor);
	drawRectangle(0,WinHeight-h-h/1.7,WinWidth,h/1.7, 1);
	
	if (!style) {
		SetPenColor(TextColor);
		drawRectangle(WinWidth/25, WinHeight/15,WinWidth/1.5,WinHeight/1.5, 1);
	}
		
}


