#ifndef _SEARCHRESULT_H_
#define _SEARCHRESULT_H_

#include <iostream>
#include "MiscProcessing.h"
using namespace std;

const int MAXIMUM_DISPLAYED_TOKEN = 5;
const int MAXIMUM_DISPLAYED_WORDS = 60;

void displayResult(int& documentNum, vector<string>& tokens);

#endif // !_SEARCHRESULT_H_