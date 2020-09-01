#ifndef _RANKFUNCTION_H_
#define _RANKFUNCTION_H_

#include <queue>
#include "MiscProcessing.h"

vector<int> andRanking(vector<vector<int>>& allDocuments);
vector<int> orRanking(vector<vector<int>>& allDocuments, bool keepOriginal = false);
vector<int> negateRanking(vector<vector<int>>& allDocuments, vector<vector<int>>& negateDocuments);

#endif