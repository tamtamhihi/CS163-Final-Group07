#ifndef _SEARCHOPERATOR_H_
#define _SEARCHOPERATOR_H_

#include "RankFunction.h"
#include "SearchResult.h"

#define AND 0
#define OR 1
#define NEGATE 2

void orOperator(vector<vector<int>>& allDocumentLists, vector<string>& tokens);
void andOperator(vector<vector<int>>& allDocumentLists, vector<string>& tokens);
void negateOperator(vector<vector<int>>& allDocumentLists, vector<vector<int>>& negateDocumentLists, vector<string>& tokens);

#endif