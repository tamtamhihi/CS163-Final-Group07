#include "SearchOperator.h"
#include "RankFunction.h"
#include "SearchResult.h"
#include "gui.h"

void search() {
	setTextColor(LIGHTBLUE);
	Node* root = newNode();
	retrieve(root);
	string query = "";
	system("cls");
	drawTitle();
	drawSearchBox();
	getQuery(query);
	while (query != "exit") {
		operatorAnd(root, query);
		cin.get();
		system("cls");
		drawTitle();
		drawSearchBox();
		query.clear();
		getQuery(query);
	}
	deleteTrie(root);
}

void getQuery(string& query) {
	char c = cin.get();
	while (c != '\n') { // c != '\n'
		if (c == 8) { // c == backspace
			if (!query.empty()) {
				query.pop_back();
				cout << "\b \b";
			}
			c = cin.get();
			continue;
		}
		query += c;
		c = cin.get();
	}
	COORD searchResult;
	searchResult.X = 0;
	searchResult.Y = 15;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), searchResult);
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
