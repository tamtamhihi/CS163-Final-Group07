#include "History.h"

// This function loads log file and is executed only once every running program.
void loadHistory(Node*& root, vector<string>& logs) {
	ifstream log("log.txt");
	if (log) {
		string query, word;
		stringstream queryStream;
		int count = 0;
		while (getline(log, query)) {
			logs.push_back(query);
			queryStream << query;
			Node* currentNode = root;
			while (queryStream >> word) {
				stringlower(word);
				currentNode = addWordReturnFinal(word, currentNode);
			}
			currentNode->isWord = count++;
			queryStream.clear();
		}
		log.close();
	}
}

// This function digs deeper into the history Trie to find
// suitable logs for a query.
vector<string> getLogsFromQuery(Node*& currentNode, Node*& history, vector<string>& logs) {
	vector<string> logResults;
	stack<Node*> logStack;
	Node* node = currentNode;
	logStack.push(node);
	while (!logStack.empty()) {
		node = logStack.top();
		logStack.pop();
		if (node->isWord != -1) {
			logResults.push_back(logs[node->isWord]);
			if (logResults.size() == MAXIMUM_HISTORY_CAPACITY)
				break;
		}
		for (auto& child : node->child)
			logStack.push(child.second);
	}
	return logResults;
}

// This function add query to log.txt.
void addLog(string& query) {
	ofstream log("log.txt", ios::app);
	if (log) {
		log << query << "\n";
		log.close();
	}
}