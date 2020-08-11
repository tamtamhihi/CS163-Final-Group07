#include "SearchOperator.h"
#include "RankFunction.h"
#include "SearchResult.h"

void search() {
	Node* root = newNode();
	retrieve(root);
	string query;
	cout << "Search File: ";
	getline(cin, query);
	operatorAnd(root, query);
	deleteTrie(root);
}

void operatorAnd(Node*& root, string& query) {
	stringstream queryStream(query);
	string word;

	// Load stopwords.
	Node* stopwordRoot = newNode();
	loadStopwords(stopwordRoot);

	// A list of all document lists containing each word in query.
	vector<vector<int>> allDocumentLists;
	vector<string> tokens;

	// Load each word in query and add corresponding document list.
	while (queryStream >> word) {
		if (!extractWord(word) || isStopword(stopwordRoot, word))
			continue;
		vector<int> documentList = findDocumentList(root, word);
		if (!documentList.empty()) {
			tokens.push_back(word);
			allDocumentLists.push_back(documentList);
		}
	}

	vector<int> searchResult = andRanking(allDocumentLists);
	cout << "Top " << searchResult.size() << " results:\n\n";
	for (int document : searchResult) {
		//cout << document << " ";
		displayResult(document, tokens);
	}

	deleteTrie(stopwordRoot);
}
