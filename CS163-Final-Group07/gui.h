#ifndef _GUI_H_
#define _GUI_H_

#define CERROR 12
#define CTEXT 14
#define CBTN 170
#define CINPUTFIELD 47
#define LIGHTBLUE 27
#define WHITE 8
#define YELLOW 174
#define RED 108
#define GREEN 186
#define GREY 119
#define PURPLE 77
#define WHITE 255

#include <iostream>
#include <string>
#include <Windows.h>
#include <windef.h>
using namespace std;

void setTextColor(int color);
void drawTitle();
void drawSearchBox();

#endif