#ifndef _RANKFUNCTION_H_
#define _RANKFUNCTION_H_

#include <queue>
#include "MiscProcessing.h"

vector<int> andRanking(vector<vector<int>>& allDocuments, int cnt);
vector<int> orRanking(vector<vector<int>>& allDocuments, int cnt);
vector<int> negateRanking(vector<vector<int>>& allDocuments, int cnt);
vector<int> intitleRanking(Node* root, Node* stopwordRoot, vector<string>& tokens, vector<vector<int>>& allDocumentLists, string& word, bool use, stringstream& ss);

#endif