#pragma once

#include "MiscProcessing.h"
#include "Indexing.h"

#define AND      1
#define OR       2
#define EXACT    3
#define WILDCARD 4

// These operators do not interfere with the rest.
// It follows the same rules: if we detect such operators ('-', "intitle:", "filetype")
#define NEGATE   5
#define PRICE    8
#define HASHTAGS 9
#define RANGE   10

#define INTITLE  6
#define FILETYPE 7
#define SYNONYM 11

int filter(string& word);
string getDocumentPath(int documentNum);
vector<string> getSynonym(string& queryWord, vector<vector<string>>& synonyms);
vector<int> andMatch(Node*& root, vector<string>& tokens);
vector<int> findExactMatchPosition(int documentNum, vector<string>& tokens);
void findAndOr(string& query, vector<string>& queryTokens, vector<int>& andOrPositions);
int isTokenInList(string& token, vector<string>& tokens);