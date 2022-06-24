#ifndef COLOR_H
#define COLOR_H


extern int color;
extern double WinWidth, WinHeight;

typedef struct color{
	char up[20];
	char mid[20];
	char down[20];
	char text[20];
	char letter[20];
	char button[20];
	char lable[20];
}Color_;
extern Color_ c1,c2,c;

void Color();
void colorstyle();
void fillcolors(char* BackgroundColor,char* TextColor,char*UpColor,char*MiddleColor); 

#endif
