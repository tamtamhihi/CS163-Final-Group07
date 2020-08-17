#include "SearchOperator.h"
#include "RankFunction.h"
#include "SearchResult.h"
#include "gui.h"

void search() {
	// set color
	setTextColor(LIGHTBLUE);

	// retrieve data from PreprocessedData/index.txt
	Node* root = newNode();
	retrieve(root);

	// Load stopwords.
	Node* stopwordRoot = newNode();
	loadStopwords(stopwordRoot);

	// gui
	string query = "";
	system("cls");
	drawTitle();
	drawSearchBox();

	getQuery(query);
	vector<string> words;

	// integer for different search operators
	// oper = 0 : AND (default)
	// oper = 1 : OR
	// oper = 2 : Manchester -United
	// ... (following project planner)
	// oper = 11 : Synonyms: ~set
	int oper = 0;

	while (query != "exit") {

		// get a list of keywords in query and get correct operator
		// this function assume users just input 2 words
		filter(words, query, oper);

		// A list of all document lists containing each word in query.
		vector<vector<int>> allDocumentLists;
		vector<string> tokens;

		// Load each word in query and add corresponding document list.
		for (auto& word : words) {
			if (!extractWord(word) || isStopword(stopwordRoot, word))
				continue;
			vector<int> documentList = findDocumentList(root, word);
			if (!documentList.empty()) {
				tokens.push_back(word);
				allDocumentLists.push_back(documentList);
			}
		}

		switch (oper) {
		case 1:
			operatorOr(root, allDocumentLists, tokens);
			break;
		case 2:
			negateOperator(root, allDocumentLists, tokens);
			break;
		default:
			operatorAnd(root, allDocumentLists, tokens);
		}

		cin.get();
		system("cls");
		drawTitle();
		drawSearchBox();
		query.clear();
		getQuery(query);
	}
	// if we have done searching, free up all memories
	deleteTrie(stopwordRoot);
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

void stringlower(string& word) {
	for (auto& c : word)
		c = tolower(c);
}
// This function will split query into words
// return a list of keywords in query
// and also change operator to the correct type
void filter(vector<string>& words, string& query, int& oper) {
	stringstream queryStream;
	string word;

	words.clear();
	queryStream.clear();
	oper = 0;

	// split query to words in order to filter different search operator
	queryStream.str(query);
	while (queryStream >> word) {
		stringlower(word);
		if (word == "or")
			oper = 1;
		else if (word[0] == '-') {
			oper = 2;
			words.push_back(word.substr(1, word.size() - 1));
			continue;
		}
		// words keep query keywords
		words.push_back(word);
	}
}


void operatorOr(Node*& root, vector<vector<int>> allDocumentLists, vector<string> tokens) {
	vector<int> searchResult = orRanking_v2(allDocumentLists);
	cout << "Top " << searchResult.size() << " results:\n\n";
	for (int document : searchResult) {
		displayResult(document, tokens);
	}
}

void operatorAnd(Node*& root, vector<vector<int>> allDocumentLists, vector<string> tokens) {
	vector<int> searchResult = andRanking(allDocumentLists);
	cout << "Top " << searchResult.size() << " results:\n\n";
	for (int document : searchResult) {
		displayResult(document, tokens);
	}
}

void negateOperator(Node*& root, vector<vector<int>> allDocumentLists, vector<string> tokens) {
	vector<int> searchResult = negateRanking(allDocumentLists);
	cout << "Top " << searchResult.size() << " results:\n\n";
	for (int document : searchResult) {
		displayResult(document, tokens);
	}
}