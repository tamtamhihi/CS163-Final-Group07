#ifndef _RANKFUNCTION_H_
#define _RANKFUNCTION_H_

#include <queue>
#include "MiscProcessing.h"
#include "SearchSubroutine.h"

vector<int> andRanking(
	vector<vector<int>>& allDocumentLists
);

vector<int> orRanking(
	vector<vector<int>>& allDocumentLists
);

vector<int> negateRanking(
	vector<int>& documentList, 
	vector<vector<int>>& negateDocumentLists
);

vector<int> andOrRanking(
	vector<int>& mustHave, 
	vector<int>& optional
);

vector<int> exactMatchRanking(
	Node*& root, 
	vector<string>& exactTokens
);

vector<int> wildcardRanking(
	Node*& root,
	vector<string>& tokens,
	vector<string>& queryTokens,
	int left,
	int right
);

vector<int> filetypeRanking(
	vector<int>& documentList,
	vector<string>& neededFiletypes,
	unordered_map<string, vector<int>>& filetypes
);

vector<int> intitleRanking(
	vector<int>& documentList,
	vector<string>& intitleTokens,
	unordered_map<string, vector<int>>& intitleRoot
);

#endif