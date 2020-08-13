#ifndef _SEARCHOPERATOR_H_
#define _SEARCHOPERATOR_H_

#include "gui.h"
#include "Indexing.h"
#include <conio.h>

void search();
void getQuery(string& query);
void operatorOr(Node*& root, vector<vector<int>> allDocumentLists, vector<string> tokens);
void operatorAnd(Node*& root, vector<vector<int>> allDocumentLists, vector<string> tokens);
void filter(vector<string>& words, string& query, int& oper);

#endif