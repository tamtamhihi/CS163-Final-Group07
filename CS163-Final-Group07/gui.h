#ifndef _GUI_H_
#define _GUI_H_

#define LIGHTBLUE 27
#define WHITE     8
#define YELLOW    174
#define RED       108
#define GREEN     10
#define DARKGREEN 2
#define GREY      119
#define PURPLE    77

#include <iostream>
#include <string>
#include <Windows.h>
#include <windef.h>
#include <vector>
using namespace std;

const int SEARCH_BOX_WIDTH = 58;
const int SEARCH_BOX_X = 25;
const int SEARCH_BOX_Y = 11;
const int MAXIMUM_HISTORY_CAPACITY = 4;

void setTextColor(int color);
void setCursorPosition(int x, int y);
void setCursorToResultPosition();
void setCursorToSearchBox();
void setCursorToLog();
void setCursorToInputPosition(int queryLength);

void drawTitle();
void drawSearchBox();

void setTextToSearchBox(string& query);
void displayHistory(vector<string>& logs, int hightlighted = -1);
void clearLog();

#endif