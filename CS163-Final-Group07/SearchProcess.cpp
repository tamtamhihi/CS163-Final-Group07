#include "SearchProcess.h"

// This is the whole searching process, including
// retrieving index, search until terminating and
// freeing memories.
void search() {
	setTextColor(LIGHTBLUE);

	// Retrieve Index tree and Stopwords into 2 Tries.
	Node* root = newNode();
	retrieve(root);
	Node* stopwordRoot = newNode();
	loadStopwords(stopwordRoot);
	Node* history = newNode();
	vector<string> logs;
	loadHistory(history, logs);

	// Draw GUI.
	system("cls");
	drawTitle();
	drawSearchBox();

	// Get query.
	string query = "";
	getQuery(query, history, logs);
	vector<string> words;

	// Integer for different search operators
	// oper = 0 : AND (default)
	// oper = 1 : OR
	// oper = 2 : NEGATE
	// ... (following project planner)
	// oper = 11 : Synonyms: ~set
	int oper = AND;

	while (query != "exit") {
		// Find operator and lower query to different words.
		oper = filter(words, query);

		// A list of all document lists containing each extracted word in query.
		vector<vector<int>> allDocumentLists;
		vector<vector<int>> negateDocumentLists;

		// List of tokens that should be highlighted when printing results.
		vector<string> tokens;

		// Load each word in query and add corresponding document list.
		// All query words will be added to tokens to highlight
		// but only non-stopwords will be passed to allDocumentLists.
		if (oper == NEGATE) {
			bool negate = false;
			for (auto& word : words) {
				if (word == "-")
					negate = true;
				if (!extractWord(word))
					continue;
				bool stopword = isStopword(stopwordRoot, word);
				vector<int> documentList = findDocumentList(root, word);
				if (!documentList.empty()) {
					if (!negate) {
						tokens.push_back(word);
						if (!stopword)
							allDocumentLists.push_back(documentList);
					}
					else if (!stopword)
						negateDocumentLists.push_back(documentList);
				}
			}
		}
		else {
			for (auto& word : words) {
				if (!extractWord(word))
					continue;
				tokens.push_back(word);
				if (isStopword(stopwordRoot, word))
					continue;
				vector<int> documentList = findDocumentList(root, word);
				if (!documentList.empty())
					allDocumentLists.push_back(documentList);
			}
		}

		// Process search operation.
		switch (oper) {
		case OR:
			orOperator(allDocumentLists, tokens);
			break;
		case NEGATE:
			negateOperator(allDocumentLists, negateDocumentLists, tokens);
			break;
		default:
			andOperator(allDocumentLists, tokens);
		}

		// Terminate this search results.
		cin.get();
		words.clear();
		query.clear();

		// Restart GUI.
		system("cls");
		drawTitle();
		drawSearchBox();

		// Get new query.
		getQuery(query, history, logs);
	}

	// If we have done searching, free up all memories.
	// Since we use unordered_map to index, the index 
	// file might not be sorted. For better appearances 
	// we would rewrite index file from the ROOT trie.
	ifstream propsFile(PROPS_FILE);
	if (propsFile) {
		int sorted;
		propsFile >> sorted;
		if (!sorted) {
			ofstream indexFile(INDEX_FILE);
			string word = "";
			printIndexToFile(indexFile, root, word);
			indexFile.close();
		}
		propsFile.close();
	}
	deleteTrie(stopwordRoot);
	deleteTrie(root);
	deleteTrie(history);
}

// This function gets query from search box. Query will be lowercased.
void getQuery(string& query, Node*& history, vector<string>& logs) {
	char c = _getch();
	Node* current = history;
	vector<string> logResults;
	int suggestionPos = -1, cursorPos = 0;
	while (c != 13) { // c != '\n'
		if (int(c) == 0 || int(c) == 0xE0) {
			cout << "^";
			cursorPos++;
			c = _getch();
			if (int(c) == LEFT && cursorPos > 0) {
				cout << "\b";
				--cursorPos;
			}
			else if (int(c) == RIGHT && cursorPos < query.size()) {
				cout << query[cursorPos];
				cursorPos++;
			}
			else if (int(c) == UP && suggestionPos != -1) {
				suggestionPos--;
				if (suggestionPos == -1) {
					displayHistory(logResults);
					setCursorToInputPosition(query.size());
				}
				else {
					displayHistory(logResults, suggestionPos);
					setTextToSearchBox(logs[suggestionPos]);
				}
			}
			else if (int(c) == DOWN && suggestionPos < MAXIMUM_HISTORY_CAPACITY) {
				suggestionPos++;
				displayHistory(logResults, suggestionPos);
			}
			c = _getch();
			continue;
		}
		if (c == 8) { // c == backspace
			if (!query.empty()) {
				cout << "\b \b";
				query.pop_back();
				current = history;
				if (query.empty()) {
					logResults.clear();
					for (int i = max(0, logs.size() - MAXIMUM_HISTORY_CAPACITY); i < logs.size(); ++i)
						logResults.push_back(logs[i]);
					displayHistory(logResults);
					setCursorToInputPosition(0);
				}
				else {
					for (char& c : query)
						if (c != ' ')
							moveCurrentNode(current, c);
					logResults = getLogsFromQuery(current, history, logs);
					displayHistory(logResults);
					setCursorToInputPosition((int)query.size());
				}
				cursorPos--;
			}
			c = _getch();
			continue;
		}
		cout << c;
		tolower(c);
		if (cursorPos < query.size()) {
			query[cursorPos] = c;
			current = history;
			for (char& c : query)
				if (c != ' ')
					moveCurrentNode(current, c);
		}
		else {
			query += c;
			if (c != ' ')
				moveCurrentNode(current, c);
		}
		logResults = getLogsFromQuery(current, history, logs);
		displayHistory(logResults);
		setCursorToInputPosition((int)query.size());
		cursorPos++;
		c = _getch();
	}
	cout << "\n\n";
	clearLog();

	// Mark the new query as log.
	logs.push_back(query);
	current->isWord = logs.size() - 1;
	addLog(query);
	setCursorToResultPosition();
}

// This function splits query into words (without extracting)
// to produce a list of keywords and return the type of operator.
int filter(vector<string>& words, string& query) {
	stringstream queryStream(query);
	string word;
	int oper = AND; // Default operator

	while (queryStream >> word) {
		if (word == "or")
			oper = OR;
		else if (word[0] == '-') {
			oper = NEGATE;
			words.push_back("-");
			if (words.size() != 1)
				words.push_back(word.substr(1, word.size() - 1));
			continue;
		}
		words.push_back(word);
	}
	return oper;
}