#ifndef _SEARCHRESULT_H_
#define _SEARCHRESULT_H_

#include "gui.h"
#include "MiscProcessing.h"
using namespace std;

#define MATCHED 100
const int MAXIMUM_DISPLAYED_TOKEN = 10;
const int MAXIMUM_DISPLAYED_WORDS = 70;
const int MAXIMUM_FILE_WORDCOUNT = 10000;

void displayResult(int& documentNum, vector<string>& tokens);

#endif // !_SEARCHRESULT_H_