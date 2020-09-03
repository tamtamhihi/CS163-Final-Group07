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
		stringstream ss(query);
		vector<vector<int>> allDocumentLists;
		vector<string> tokens;
		int isOp = 0;
		int op = 0, cnt = 0;
		vector<int> searchResult;
		string word, sub;
		// tom tat:
		// moi lan dua vao 1 tu moi tu cu update documentList va searchResult
		while (ss.good() || isOp) {
			if (!isOp || word == "and" || word == "or") {
				ss >> word;
				stringlower(word);
				op = filter(word);
			}
			if (op) {
				isOp = op;
				op = 0;
				continue;
			}
			if (!extractWord(word) || isStopword(stopwordRoot, word)) {
				continue;
			}

			vector<int> documentList = findDocumentList(root, word);
			if (!documentList.empty()) {
				tokens.push_back(word);
				allDocumentLists.push_back(documentList);
				searchResult = documentList;
			}

			if (isOp) {
				switch (isOp) {
				case 1:
					searchResult = andRanking(allDocumentLists, cnt);
					break;
				case 2:
					searchResult = negateRanking(allDocumentLists, cnt);
					if (tokens.size() == 1)
						tokens.clear();
					break;
				case 3:
					bool use = false;
					if (tokens.back() == word)
						use = true;
					searchResult = intitleRanking(root, stopwordRoot, tokens, allDocumentLists, word, use, ss);
					break;
				}


				allDocumentLists.erase(allDocumentLists.begin());
				--cnt;
				isOp = 0;
			}
			else if (cnt > 0) {
				searchResult = orRanking(allDocumentLists, cnt);
				allDocumentLists.erase(allDocumentLists.begin());
				--cnt;
			}
			++cnt;
		}

		// display result here
		cout << "Top " << searchResult.size() << " results:\n\n";
		for (int document : searchResult) {
			if (tokens.size())
				displayResult(document, tokens);
			else
				cout << document << ".txt" << "\n";
		}

		cin.get();
		system("cls");
		drawTitle();
		drawSearchBox();
		query.clear();
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
	while (c != 13) {
		if (int(c) == ALPHA) {
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
				}
				else {
					displayHistory(logResults, suggestionPos);
					setTextToSearchBox(logResults[suggestionPos]);
					for (int d = query.size(); d < logResults[suggestionPos].size(); ++d)
						moveCurrentNode(current, logResults[suggestionPos][d]);
					query = logResults[suggestionPos];
					cursorPos = query.size();
				}
				setCursorToInputPosition(query.size());
			}
			else if (int(c) == DOWN && !logResults.empty() && suggestionPos < MAXIMUM_HISTORY_CAPACITY - 1) {
				suggestionPos++;
				displayHistory(logResults, suggestionPos);
				setTextToSearchBox(logResults[suggestionPos]);
				for (int d = query.size(); d < logResults[suggestionPos].size(); ++d)
					moveCurrentNode(current, logResults[suggestionPos][d]);
				query = logResults[suggestionPos];
				cursorPos = query.size();
				setCursorToInputPosition(query.size());
			}
			c = _getch();
			continue;
		}
		if (c == 8) { // c == backspace
			if (cursorPos > 0) {
				cout << "\b \b";
				if (cursorPos == query.size())
					query.pop_back();
				else
					query.erase(query.begin() + cursorPos - 1);
				setTextToSearchBox(query);
				cursorPos--;
				for (char d : query) {
					d = tolower(d);
					if (d != ' ')
						moveCurrentNode(current, d);
				}
				current = history;
				if (query.empty()) {
					logResults.clear();
					// Display most recent searches.
					for (int i = max(0, logs.size() - MAXIMUM_HISTORY_CAPACITY); i < logs.size(); ++i)
						logResults.push_back(logs[i]);
					displayHistory(logResults);
					setCursorToInputPosition(0);
				}
				else {
					for (char d : query) {
						d = tolower(d);
						if (d != ' ')
							moveCurrentNode(current, d);
					}
					logResults = getLogsFromQuery(current, history, logs);
					displayHistory(logResults);
					setCursorToInputPosition(cursorPos);
				}
			}
			/*
			if (!query.empty()) {
				cout << "\b \b";
				query.pop_back();
				current = history;
				if (query.empty()) {
					logResults.clear();
					// Display most recent searches.
					for (int i = max(0, logs.size() - MAXIMUM_HISTORY_CAPACITY); i < logs.size(); ++i)
						logResults.push_back(logs[i]);
					displayHistory(logResults);
					setCursorToInputPosition(0);
				}
				else {
					for (char d : query) {
						d = tolower(d);
						if (d != ' ')
							moveCurrentNode(current, d);
					}
					logResults = getLogsFromQuery(current, history, logs);
					displayHistory(logResults);
					setCursorToInputPosition((int)query.size());
				}
				cursorPos--;
			}
			*/
			suggestionPos = -1;
			c = _getch();
			continue;
		}
		cout << c;
		if (cursorPos < query.size()) {
			query[cursorPos] = c;
			suggestionPos = -1;
			current = history;
			for (char d : query) {
				d = tolower(d);
				if (d != ' ')
					moveCurrentNode(current, d);
			}
		}
		else {
			query += c;
			suggestionPos = -1;
			c = tolower(c);
			if (c != ' ')
				moveCurrentNode(current, c);
		}
		logResults = getLogsFromQuery(current, history, logs);
		displayHistory(logResults);
		cursorPos++;
		setCursorToInputPosition(cursorPos);
		c = _getch();
	}
	cout << "\n\n";
	clearLog();

	// Mark the new query as log.
	if (query != "exit") {
		logs.push_back(query);
		current->isWord = logs.size() - 1;
		addLog(query);
	}
	stringlower(query);
	setCursorToResultPosition();
}

// This function splits query into words (without extracting)
// to produce a list of keywords and return the type of operator.
int filter(string& word) {
	stringlower(word);
	string sub = word.substr(0, 8);
	int oper = 0;


	if (word == "and") {
		oper = 1;

	}
	else if (word[0] == '-') {
		oper = 2;
		word = word.substr(1, word.size() - 1);
	}
	else if (sub == "intitle:") {
		word = word.substr(8, word.size() - 8);
		oper = 3;
	}
	else if (sub == "filetype") {
		oper = 4;
		word = word.substr(9, word.size() - 9);
	}
	else if (word[0] == '"') {


	}
	return oper;
}
