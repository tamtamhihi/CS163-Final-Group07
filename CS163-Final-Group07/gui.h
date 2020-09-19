#ifndef _GUI_H_
#define _GUI_H_

#define LIGHTBLUE  27
#define WHITE       8
#define YELLOW    174
#define RED       108
#define GREEN      10
#define DARKGREEN   2
#define GREY      119
#define PURPLE     77
#define MAGENTA    13

#include <iostream>
#include <string>
#include <Windows.h>
#include <windef.h>
#include <vector>
using namespace std;

const int SEARCH_BOX_WIDTH = 58;
const int MENU_BOX_WIDTH = 16;
const int INPUT_BOX_WIDTH = 110;
const int INPUT_BOX_HEIGHT = 2;
const int SEARCH_BOX_X = 25;
const int SEARCH_BOX_Y = 12;
const int INPUT_BOX_X = 2;
const int INPUT_BOX_Y = 14;
const int RESULT_X = 0;
const int RESULT_Y = 16;

void setTextColor(int color);
void setCursorPosition(int x, int y);
void setCursorToResultPosition();
void setCursorToSearchBox();
void setCursorToLog();
void setCursorToInputPosition(int queryLength);

void drawTitle();
void drawMenu(int highlightedChoice = 0);
void drawSearchBox();
void drawInputBox(int height = INPUT_BOX_HEIGHT);

void setTextToSearchBox(string& query);
void displayHistory(vector<string>& logs, int hightlighted = -1);
void clearLog();

#endif