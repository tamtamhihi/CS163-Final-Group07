#pragma once
#include "MiscProcessing.h"
#include <stack>

#define MAXIMUM_HISTORY_CAPACITY 4

void loadHistory(Node*& root, vector<string>& logs);
vector<string> getLogsFromQuery(Node*& currentNode, Node*& history, vector<string>& logs);
void addLog(string& query);