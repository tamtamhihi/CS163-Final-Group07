#ifndef _RANKFUNCTION_H_
#define _RANKFUNCTION_H_

#include <queue>
#include "MiscProcessing.h"

bool docInPair(vector<pair<int, int>> countDocuments, int doc);
vector<int> orRanking(vector<vector<int>>& allDocuments);
vector<int> andRanking(vector<vector<int>>& allDocuments);
vector<int> orRanking_v2(vector<vector<int>>& allDocuments);
vector<int> negateRanking(vector<vector<int>> allDocuments);

#endif