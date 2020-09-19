#include "SearchProcess.h"

void initiateSearchEngine() {
	drawTitle();

	cout << "\n\n\t\t\t\t\t\tLoading....\n\n";
	Node* root = newNode(), * stopwordRoot = newNode(), * history = newNode();
	vector<string> logs;
	vector<vector<string>> synonyms;
	unordered_map<string, vector<int>> filetypes, intitleRoot;
	retrieve(root);
	loadStopwords(stopwordRoot);
	loadHistory(history, logs);
	loadSynonyms(synonyms);
	loadFiletypes(filetypes);

	system("cls");
	drawTitle();
	drawMenu();

	char c;
	c = _getch();
	int highlightedOption = 0;
	bool exitProgram = false;
	while (true) {
		if (c == 13) {
			system("cls");
			drawTitle();
			switch (highlightedOption) {
			case 0:
				search(root, intitleRoot, stopwordRoot, history, logs, synonyms, filetypes);
				break;
			case 1:
				displayGuides();
				break;
			case 2:
				addDocument(root, intitleRoot, stopwordRoot, filetypes);
				break;
			case 3:
				exitProgram = true;
				break;
			}
			system("cls");
			drawTitle();
			if (exitProgram)
				break;
			drawMenu(highlightedOption);
		}
		else if (c == ALPHA) {
			c = _getch();
			if (c == UP && highlightedOption) {
				highlightedOption--;
				system("cls");
				drawTitle();
				drawMenu(highlightedOption);
			}
			else if (c == DOWN && highlightedOption < 3) {
				highlightedOption++;
				system("cls");
				drawTitle();
				drawMenu(highlightedOption);
			}
		}
		c = _getch();
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
		ofstream propsFileOut(PROPS_FILE);
		propsFileOut << 1 << "\n";
		propsFileOut.close();
	}
	deleteTrie(stopwordRoot);
	deleteTrie(root);
	deleteTrie(history);
	system("cls");
	drawTitle();
	cout << "\n\t\t\t\t\t    Goodbyeeeeee wibu\n\n\n\n";
}

// This function displays guides into a new screen.
void displayGuides() {
	cout << "\n\n";
	setTextColor(GREEN);
	cout << "\t\tHello this is Seven Search.\n\n";
	cout << "\t\tA few interesting tips:\n\n";
	vector<string> guides = {
		"- Use -token or -\"many different tokens\" to eliminate search results containing such tokens",
		"- Use \"some exact tokens\" to find results containing the exact match of these tokens",
		"- Use some wild * card tokens to find search results that fill in the missing part",
		"- Use $ or # to search for price and hashtags",
		"- Use $100..$500 together with other keys to search for a range of price from $100 to $500. Remember to use only 2 dots",
		"- Use intitle:token or intitle:\"many tokens\" to search for results with title containing such tokens",
		"- Use filetype:txt or filetype:\"txt, csv, pdf\" to search for certain filetypes",
		"- Use ~token to search for all results with synonyms of that token",
		"- Use AND or OR to connect different operators"
	};
	vector<int> colors = { LIGHTBLUE, YELLOW, RED, MAGENTA, GREEN };
	vector<vector<int>> highlighted = {
		{2, 4, 5, 6},
		{2, 3, 4},
		{2, 3, 4, 5, 6},
		{2, 4},
		{2},
		{2, 4, 5},
		{2, 4, 5, 6},
		{2},
		{2, 4}
	};
	setTextColor(GREY);
	srand(time(NULL));
	for (int i = 0; i < guides.size(); ++i) {
		int wordCount = 0, lineCount = 0;
		cout << "\t\t\t";
		string word;
		stringstream guide(guides[i]);
		while (guide >> word) {
			int index = lower_bound(highlighted[i].begin(), highlighted[i].end(), wordCount) - highlighted[i].begin();
			if (index != highlighted[i].size() && highlighted[i][index] == wordCount) {
				int color = rand() % colors.size();
				setTextColor(colors[color]);
				cout << word;
				setTextColor(GREY);
			}
			else
				cout << word;
			cout << " ";
			lineCount += word.size() + 1;
			if (lineCount >= 50) {
				cout << "\n\t\t\t";
				lineCount = 0;
			}
			++wordCount;
		}
		cout << "\n\n";
	}
	setTextColor(GREEN);
	cout << "\n\t\tPress anything to return.";
	char c = _getch();
}


// This function prompts a filepath to a file that contains
// all document paths to add to index.
void addDocument(Node*& root, unordered_map<string, vector<int>>& intitleRoot, Node*& stopwordRoot, unordered_map<string, vector<int>>& filetypes) {
	string filepath = "";
	cout << "\n\tEnter filepath to the file that contains filepath to all documents (exit to comeback):\n";
	drawInputBox();
	getline(cin, filepath);
	setCursorPosition(INPUT_BOX_X, INPUT_BOX_Y + INPUT_BOX_HEIGHT + 1);
	if (filepath != "exit") {
		cout << "\tAdding to index...";
		addNewDocumentsToIndex(root, intitleRoot, stopwordRoot, filetypes, filepath);
		cout << "\n\tFinished.\n";
	}
	cout << "\tPress anything to return. ";
	char c = _getch();
}


// This is the whole searching process, including
// retrieving index, search until terminating and
// freeing memories.
void search(
	Node*& root, 
	unordered_map<string, vector<int>>& intitleRoot,
	Node*& stopwordRoot, 
	Node*& history, 
	vector<string>& logs, 
	vector<vector<string>>& synonyms,
	unordered_map<string, vector<int>>& filetypeList) {
	setTextColor(LIGHTBLUE);

	// Draw GUI.
	system("cls");
	drawTitle();
	drawSearchBox();

	vector<string> tokens, queryTokens;
	vector<int> andOrPositions;
	vector<int> leftDocuments, rightDocuments;
	vector<vector<int>> allDocuments;
	vector<string> filetypes, intitle;

	// Get query.
	string query = "";
	getQuery(query, history, logs);

	while (query != "exit") {
		tokens.clear();
		queryTokens.clear();
		andOrPositions.clear();
		leftDocuments.clear();
		rightDocuments.clear();
		allDocuments.clear();
		filetypes.clear();
		intitle.clear();

		auto start = chrono::high_resolution_clock::now();

		// Split query into string list of queryTokens
		// and find positions of "and", "or".
		findAndOr(query, queryTokens, andOrPositions);

		// Add an extra position after the last index of queryTokens.
		andOrPositions.push_back(queryTokens.size());

		int numOfAndOr = andOrPositions.size(), left = 0, right = andOrPositions[0] - 1;

		// We now consider each subarray of strings between "and", "or" keywords
		// to be SUBQUERY to be processed independently. By processing, we mean that:
		//    + Put intitle and filetype into corresponding vector for later use.
		//    + Or ranking all keywords in same subquery.
		//    + Negate ranking if any.

		// Process the first subquery (even if there's no "and", "or", we still get the whole
		// query as the first subquery because we've added the position after last index
		// into andOrPositions. The result is saved to leftDocuments.
		processSubquery(root, stopwordRoot, synonyms, intitle, filetypes, tokens, leftDocuments, queryTokens, left, right);
		
		// For every "and", "or"
		for (int i = 0; i < numOfAndOr - 1; ++i) {
			left = andOrPositions[i] + 1;
			right = andOrPositions[i + 1] - 1;

			// We process the next subquery to rightDocuments.
			processSubquery(root, stopwordRoot, synonyms, intitle, filetypes, tokens, rightDocuments, queryTokens, left, right);
			allDocuments.push_back(leftDocuments);
			allDocuments.push_back(rightDocuments);

			// Based on "and" or "or" to do the corresponding ranking with
			// leftDocuments and rightDocuments, then save it to be the new
			// leftDocuments. The final results of the whole query will also
			// be saved to leftDocuments.
			if (queryTokens[andOrPositions[i]] == "and")
				rightDocuments = andRanking(allDocuments);
			else
				rightDocuments = orRanking(allDocuments);

			allDocuments.clear();
		}

		// Sort the tokens and intitle tokens list for better finding of tokens.
		sort(tokens.begin(), tokens.end());
		sort(intitle.begin(), intitle.end());

		// Previous ranking based on "and", "or" does not look into
		// intitle and filetypes. Here we do it on leftDocuments.
		leftDocuments = intitleRanking(leftDocuments, intitle, intitleRoot);
		leftDocuments = filetypeRanking(leftDocuments, filetypes, filetypeList);

		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

		// Display at most 10 results.
		cout << "About "<< leftDocuments.size() << " results in " << duration.count() << "ms\n\n";
		int count = 0;
		for (int document : leftDocuments) {
			displayResult(document, tokens, intitle);
			count++;
			if (count == MAXIMUM_DISPLAYED_SEARCH_RESULTS)
				break;
		}

		// Get new query.
		_getch();
		system("cls");
		drawTitle();
		drawSearchBox();
		query.clear();
		getQuery(query, history, logs);
	}
}

// This function gets query from search box until the user presses "\n".
// The returned query will be in lowercase.
void getQuery(string& query, Node*& history, vector<string>& logs) {
	// This pointer is used to keep track of the current query
	// in the HISTORY trie to find the suitable logs.
	Node* current = history;
	vector<string> logResults;

	// Keep track whether the user has pressed 
	// UP/DOWN/LEFT/RIGHT arrow key.
	//   + suggestionPos corresponds with the index of current highlighted log
	//   + cursorPos corresponds with the number of char in front of cursor
	int suggestionPos = -1, cursorPos = 0;

	char c = _getch();

	while (true) {
		// We only stop when user presses "\n"
		// and the query is not empty.
		if (c == 13) {
			if (!query.empty())
				break;
			else {
				c = _getch();
				continue;
			}
		}

		// Detect arrow key. Arrow keys contain 2 consecutive characters,
		// the first one is ALPHA (key = -32) and the second one is 
		// either H, P, K, or M.

		// When user uses arrow keys, we do not change pointer current 
		// in HISTORY trie because throughout the whole process, the logResults
		// will not be changed, except for the highlighted one.
		// current is only changed when user adds, deletes, or replaces
		// a character.

		if (int(c) == ALPHA) {
			c = _getch();
			// Move left only when there exists some chars
			// in front of current cursor.
			if (int(c) == LEFT && cursorPos > 0) {
				cout << "\b";
				--cursorPos;
			}
			// Move right only when the cursor is not at
			// the end of the current query.
			else if (int(c) == RIGHT && cursorPos < query.size()) {
				cout << query[cursorPos];
				cursorPos++;
			}
			// Move up only when there is a currently highlighted log.
			else if (c == UP && suggestionPos != -1) {
				suggestionPos--;
				// If moving up corresponds to the search box,
				// we display logResults again with no highlight.
				if (suggestionPos == -1)
					displayHistory(logResults);
				// If moving up to another log, we change the highlight,
				// set that log to the search box.
				else {
					displayHistory(logResults, suggestionPos);
					setTextToSearchBox(logResults[suggestionPos]);
					query = logResults[suggestionPos];
					cursorPos = min(cursorPos, query.size());
				}
				setCursorToInputPosition(cursorPos);
			}
			// Move down only when there's at least 1 log below.
			else if (c == DOWN && suggestionPos < (int)logResults.size() - 1) {
				suggestionPos++;
				displayHistory(logResults, suggestionPos);
				setTextToSearchBox(logResults[suggestionPos]);
				query = logResults[suggestionPos];
				cursorPos = min(cursorPos, query.size());
				setCursorToInputPosition(cursorPos);
			}
			c = _getch();
			continue;
		}
		
		// When user presses backspace to delete
		if (c == 8) {
			// Only delete when there exists at least 1 character
			// in front of cursor.
			if (cursorPos > 0) {
				cout << "\b \b";
				query.erase(query.begin() + cursorPos - 1);
				setTextToSearchBox(query);
				cursorPos--;
				// Reset current in HISTORY trie.
				current = history;
				for (char d : query) {
					d = tolower(d);
					if (d != ' ')
						moveCurrentNode(current, d);
				}
				// If the search box is now empty, we display
				// the most recent logs. Else, we search for 
				// suitable logs again.
				if (query.empty()) {
					logResults.clear();
					for (int i = max(0, logs.size() - MAXIMUM_HISTORY_CAPACITY); i < logs.size(); ++i)
						logResults.push_back(logs[i]);
					displayHistory(logResults);
					setCursorToInputPosition(0);
				}
				else {
					logResults = getLogsFromQuery(current, history, logs);
					displayHistory(logResults);
					setCursorToInputPosition(cursorPos);
				}
			}
			// Reset suggestion since logs have changed.
			suggestionPos = -1;
			c = _getch();
			continue;
		}

		// If it's a normal character (no enter, no backspace, no arrow),
		// we first print it to console.
		cout << c;
		// Replace an existing character.
		if (cursorPos < query.size()) {
			query[cursorPos] = c;
			current = history;
			for (char d : query) {
				d = tolower(d);
				if (d != ' ')
					moveCurrentNode(current, d);
			}
		}
		// Add new character.
		else {
			query += c;
			c = tolower(c);
			if (c != ' ')
				moveCurrentNode(current, c);
		}
		// Reset suggestionPos and find new logs.
		suggestionPos = -1;
		cursorPos++;
		logResults = getLogsFromQuery(current, history, logs);
		displayHistory(logResults);
		setCursorToInputPosition(cursorPos);
		c = _getch();
	}
	// Clear trailing logs.
	cout << "\n\n";
	clearLog();

	// Mark the new query as log and append to log file.
	if (query != "exit") {
		logs.push_back(query);
		current->isWord = logs.size() - 1;
		addLog(query);
	}

	// Lowercase query.
	stringlower(query);
	setCursorToResultPosition();
}

// This function processes a subquery (a subarray of the list of tokens)
// assuming that exists no "and" or "or" words in between.
void processSubquery(
	Node*& root, 
	Node*& stopwordRoot, 
	vector<vector<string>>& synonyms, 
	vector<string>& intitleTokens, 
	vector<string>& filetypes, 
	vector<string>& tokens, 
	vector<int>& resultDocuments, 
	vector<string>& query, 
	int left, int right) {

	int foundOperator = 0, j, from, to;
	vector<int> documentList;
	vector<string> exactTokens;

	// We must NOT include in the final resultDocuments any documents from negateDocumentList.
	vector<vector<int>> subqueryDocumentLists, negateDocumentLists, mustHaveDocumentLists;
	bool exact = false, wildcard = false;

	// Check if this is wild card (wild card must be dealt with in a totally different way.
	for (int i = left; i <= right; ++i) {
		if (query[i] == "*") {
			wildcard = true;
			break;
		}
	}
	// No wildcard: process each word. If we find no operator: add to subqueryDocumentLists (find as much as possible)
	// If exists operator: process that operator.
	if (!wildcard) {
		for (int i = left; i <= right; ++i) {
			foundOperator = filter(query[i]);
			switch (foundOperator) {
			case NEGATE:
				negateOperator(root, negateDocumentLists, query, i, right);
				break;
			case INTITLE:
				intitleOperator(root, mustHaveDocumentLists, intitleTokens, query, i, right);
				break;
			case FILETYPE:
				filetypeOperator(root, filetypes, query, i, right);
				break;
			case RANGE:
				rangeOperator(root, tokens, mustHaveDocumentLists, query[i]);
				break;
			case PRICE:
				priceOperator(root, tokens, mustHaveDocumentLists, query[i]);
				break;
			case EXACT:
				exactOperator(root, mustHaveDocumentLists, tokens, query, i, right);
				exact = true;
				break;
			case SYNONYM:
				synonymOperator(root, subqueryDocumentLists, tokens, synonyms, query[i]);
				break;
			default:
				defaultOperator(root, stopwordRoot, subqueryDocumentLists, tokens, query[i]);
			}
		}
		documentList = orRanking(subqueryDocumentLists);
		vector<int> andResult = andRanking(mustHaveDocumentLists);
		resultDocuments = andOrRanking(andResult, documentList);
		resultDocuments = negateRanking(resultDocuments, negateDocumentLists);
	}
	else
		resultDocuments = wildcardRanking(root, tokens, query, left, right);
}

