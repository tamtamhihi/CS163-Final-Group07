#pragma once
#include "Indexing.h"
#include "SearchOperator.h"
#include "History.h"
#include "gui.h"
#include <conio.h>

#define ALPHA -32
#define UP    72
#define DOWN  80
#define LEFT  75
#define RIGHT 77

void search();
void getQuery(string& query, Node*& history, vector<string>& logs);
int filter(string& words);