#ifndef _SEARCHOPERATOR_H_
#define _SEARCHOPERATOR_H_

#include "RankFunction.h"
#include "Indexing.h"
#include "SearchSubroutine.h"
#include <unordered_set>

void negateOperator(
	Node*& root, 
	vector<vector<int>>& negateDocumentLists, 
	vector<string>& queryTokens, 
	int& currentQueryIndex, 
	int lastIndexOfSubquery
);

void intitleOperator(
	Node*& root,
	vector<vector<int>>& mustHaveDocumentLists,
	vector<string>& intitleTokens,
	vector<string>& queryTokens,
	int& currentQueryIndex,
	int lastIndexOfSubquery
);

void filetypeOperator(
	Node*& root,
	vector<string>& filetypes,
	vector<string>& queryTokens,
	int& currentQueryIndex,
	int lastIndexOfSubquery
);

void rangeOperator(
	Node*& root, 
	vector<string>& tokens, 
	vector<vector<int>>& allDocumentLists, 
	string& queryWord
);

void priceOperator(
	Node*& root, 
	vector<string>& tokens, 
	vector<vector<int>>& mustHaveDocumentLists,
	string& queryWord
);

void exactOperator(
	Node*& root,
	vector<vector<int>>& mustHaveDocumentLists,
	vector<string>& tokens,
	vector<string>& queryTokens,
	int& currentQueryIndex,
	int lastIndexOfSubquery
);

void synonymOperator(
	Node*& root,
	vector<vector<int>>& allDocumentLists,
	vector<string>& tokens,
	vector<vector<string>>& synonyms,
	string& queryWord
);

void defaultOperator(
	Node*& root,
	Node*& stopwordRoot,
	vector<vector<int>>& allDocumentLists,
	vector<string>& tokens,
	string& queryWord
);

#endif