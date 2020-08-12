#ifndef _SEARCHRESULT_H_
#define _SEARCHRESULT_H_

#include <iostream>
#include "gui.h"
#include "MiscProcessing.h"
using namespace std;

#define MATCHED 100
const int MAXIMUM_DISPLAYED_TOKEN = 5;
const int MAXIMUM_DISPLAYED_WORDS = 60;
const int MAXIMUM_FILE_WORDCOUNT = 8000;

void displayResult(int& documentNum, vector<string>& tokens);

#endif // !_SEARCHRESULT_H_