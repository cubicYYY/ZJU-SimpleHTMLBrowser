#include <strlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h> 
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "color.h"
#include "button.h"
#include "tabs.h"
#include "renderEngine.h"
#include "types.h"
#include "simpleCSS.h"
#include "analysis.h"



#define MAX 1000

// ���������˸��ʱ��
#define CURSOR_TIMER 1
extern void partialDisplayClear(void);
extern void analysis(void);
extern string html_str;
extern string redirect;
extern float_64 shift_x,shift_y;
extern bool lazying;
extern string globalCSS;
double WinWidth, WinHeight;
char add[MAX]; //��ʱ����洢��ַ
int tabNumber=1;//�ڼ���Tab
int totaltabNumber=1;	//��ʱ������Tab
int pageFlash = 0; //�Ƿ�����ļ����ж�
int newflag = 0; //�Ƿ��������½ڵ���ж�
int Tabflag = 0;//��ǩҳ���ж�
char textbuf2[MAX][MAX];//ȫ�ֱ��������ַ���ļ�����
FILE *fp;//�ļ�ָ����Ϊhtml

struct color c1= {"1-up","1-mid","1-down","1-text","1-letter","1-button","1-lable"};
struct color c2= {"2-up","2-mid","2-down","2-text","2-letter","2-button","2-lable"};
struct color c;

void display();

void TimerEventProcess(int timerID)
{
	display();

}
void MouseEventProcess(int x, int y, int button, int event) //���¹��� 
{
	uiGetMouse(x,y,button,event);
	if (event==ROLL_DOWN) shift_y+=0.04;
	if (event==ROLL_UP) shift_y-=0.04;
	shift_y=max(shift_y,-5);
	display();
}
void KeyboardEventProcess(int key, int event) //���ҹ��� 
{
	uiGetKeyboard(key,event);
	if (key==VK_LEFT) shift_x-=0.04;
	if (key==VK_RIGHT) shift_x+=0.04;
	shift_x=max(shift_x,-1.4);
	shift_x=min(shift_x,5);
	display();

}
void CharEventProcess(char ch)
{
	uiGetChar(ch);
	display();
}

void display()
{
	Color();
	partialDisplayClear();/*ֻ�����棬�����ļ����� */
	colorstyle();
	fillcolors(c.down, c.text, c.up, c.mid);
//	DrawStyleButtons();
	Button();
	addinput();
	drawTabs();
	analysis();
	masking();
	DrawStyleButtons();
	if(newflag)
		AddAddtoLink();
}


void Main()
{
//	InitConsole();
	SetWindowTitle("��ҳ�����");
	SetWindowSize(16,9);
	InitGraphics();
	SetFont("Consolas");
	WinWidth = GetWindowWidth();
	WinHeight = GetWindowHeight();
	registerMouseEvent(MouseEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerCharEvent(CharEventProcess);
	registerTimerEvent(TimerEventProcess);
	FILE *cssfp=fopen("initial.css","r");//����ȫ��CSS
	fread(globalCSS,HTML_BUF_SIZE,sizeof(char),cssfp);
	fclose(cssfp);
	html_str=(string)GetBlock((HTML_BUF_SIZE+1)*sizeof(char));
	shift_x=0;
	lazying=FALSE;
	startTimer(CURSOR_TIMER, 30); //����������ʱ��
}



