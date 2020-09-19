#include "SearchOperator.h"

// This function reads in and processes all negated tokens from the subquery.
void negateOperator(
	Node*& root, 
	vector<vector<int>>& negateDocumentLists, 
	vector<string>& queryTokens, 
	int& currentQueryIndex, int lastIndexOfSubquery) {

	string queryWord = queryTokens[currentQueryIndex];
	vector<int> documentList;

	// If this word is only "-"
	if (queryWord.size() == 1) {
		// We will take the next token in that subquery to be the
		// negated token IF it does not contain a different operator.
		if (currentQueryIndex + 1 <= lastIndexOfSubquery) {
			int newOperator = filter(queryTokens[currentQueryIndex + 1]);
			if (!newOperator) {
				++currentQueryIndex;
				documentList = findDocumentList(root, queryTokens[currentQueryIndex]);
				if (!documentList.empty())
					negateDocumentLists.push_back(documentList);
			}
		}
	}
	else {
		// If it only has 1 word to negate (no " ")
		if (queryWord[1] != '"') {
			queryWord = queryWord.substr(1, queryWord.size() - 1);
			extractWord(queryWord);
			documentList = findDocumentList(root, queryWord);
			if (!documentList.empty())
				negateDocumentLists.push_back(documentList);
		}
		else {
			queryTokens[currentQueryIndex] = queryWord = queryWord.substr(2, queryWord.size() - 2);
			// Try to find the ending quotation mark.
			int j = currentQueryIndex;
			while (j <= lastIndexOfSubquery && (queryTokens[j].empty() || queryTokens[j].back() != '"'))
				j++;
			// If we can't find it and the first word is empty, there's no tokens to negate.
			// Else we still examine the first word (with the lonely opening qutation mark).
			if (j > lastIndexOfSubquery) {
				if (queryWord.empty())
					return;
				j = currentQueryIndex; // Make sure that only the first token (query[i]) gets examined.
			}
			for (; currentQueryIndex <= j; ++currentQueryIndex) {
				if (currentQueryIndex == j && !queryTokens[currentQueryIndex].empty() && queryTokens[currentQueryIndex].back() == '"')
					queryTokens[currentQueryIndex].pop_back();
				extractWord(queryTokens[currentQueryIndex]);
				documentList = findDocumentList(root, queryTokens[currentQueryIndex]);
				if (!documentList.empty())
					negateDocumentLists.push_back(documentList);
			}
		}
	}
}

// This function reads in and processes all intitle tokens from the subquery.
void intitleOperator(
	Node*& root, 
	vector<vector<int>>& mustHaveDocumentLists, 
	vector<string>& intitleTokens, 
	vector<string>& queryTokens,
	int& currentQueryIndex, int lastIndexOfSubquery) {

	string queryWord = queryTokens[currentQueryIndex];
	vector<int> documentList;

	// If the token is only "intitle:"
	if (queryWord.size() == 8) {
		// We will take the next token in that subquery to be the
		// intitle token IF it does not contain a different operator.
		if (currentQueryIndex + 1 <= lastIndexOfSubquery) {
			int newOperator = filter(queryTokens[currentQueryIndex + 1]);
			if (!newOperator) {
				++currentQueryIndex;
				extractWord(queryTokens[currentQueryIndex]);
				intitleTokens.push_back(queryTokens[currentQueryIndex]);
				mustHaveDocumentLists.push_back(findDocumentList(root, queryTokens[currentQueryIndex]));
			}
		}
	}
	else {
		// If it has only 1 intitle token
		if (queryWord[8] != '"') {
			queryWord = queryWord.substr(8, queryWord.size() - 8);
			extractWord(queryWord);
			intitleTokens.push_back(queryWord);
			mustHaveDocumentLists.push_back(findDocumentList(root, queryWord));
		}
		else {
			queryTokens[currentQueryIndex] = queryWord = queryWord.substr(9, queryWord.size() - 9);
			// Try to find the ending quotation mark.
			int j = currentQueryIndex;
			while (j <= lastIndexOfSubquery && (queryTokens[j].empty() || queryTokens[j].back() != '"'))
				j++;
			// If we can't find it and the first word is empty, there's no intitle tokens.
			// Else we still examine the first word (with the lonely opening qutation mark).
			if (j > lastIndexOfSubquery) {
				if (queryWord.empty())
					return;
				j = currentQueryIndex; // Make sure that only the first token is processed.
			}
			for (; currentQueryIndex <= j; ++currentQueryIndex) {
				if (currentQueryIndex == j && !queryTokens[currentQueryIndex].empty() && queryTokens[currentQueryIndex].back() == '"')
					queryTokens[currentQueryIndex].pop_back();
				if (!queryTokens[currentQueryIndex].empty()) {
					extractWord(queryTokens[currentQueryIndex]);
					intitleTokens.push_back(queryTokens[currentQueryIndex]);
					mustHaveDocumentLists.push_back(findDocumentList(root, queryTokens[currentQueryIndex]));
				}
			}
		}
	}
}

// This function reads in and processes all filetype tokens from the subquery.
void filetypeOperator(
	Node*& root, 
	vector<string>& filetypes, 
	vector<string>& queryTokens, 
	int& currentQueryIndex, int lastIndexOfSubquery) {

	string queryWord = queryTokens[currentQueryIndex];

	// If the word is only "filetype:"
	if (queryWord.size() == 9) {
		// We will take the next token in that subquery to be the
		// filetype token IF it does not contain a different operator.
		if (currentQueryIndex + 1 <= lastIndexOfSubquery) {
			int newOperator = filter(queryTokens[currentQueryIndex + 1]);
			if (!newOperator) {
				++currentQueryIndex;
				filetypes.push_back(queryTokens[currentQueryIndex]);
			}
		}
	}
	else {
		// If it has only 1 filetype token
		if (queryWord[9] != '"')
			filetypes.push_back(queryWord.substr(9, queryWord.size() - 9));
		else {
			queryTokens[currentQueryIndex] = queryWord = queryWord.substr(9, queryWord.size() - 9);
			// Try to find the ending quotation mark.
			int j = currentQueryIndex;
			while (j <= lastIndexOfSubquery && (queryTokens[j].empty() || queryTokens[j].back() != '"'))
				j++;
			// If we can't find it and the first word is empty, there's no fieltype tokens.
			// Else we still examine the first word (with the lonely opening qutation mark).
			if (j > lastIndexOfSubquery) {
				if (queryWord.empty())
					return;
				j = currentQueryIndex;
			}
			for (; currentQueryIndex <= j; ++currentQueryIndex) {
				queryWord = queryTokens[currentQueryIndex];
				if (!queryWord.empty() && (queryWord.back() == ',' || queryWord.back() == '"'))
					queryWord.pop_back();
				if (!queryWord.empty())
					filetypes.push_back(queryWord);
			}
		}
	}
}

// This function reads in and processes a range token from the subquery.
void rangeOperator(
	Node*& root, 
	vector<string>& tokens, 
	vector<vector<int>>& mustHaveDocumentLists, 
	string& queryWord) {

	vector<int> documentList;
	vector<string> allQueryFromRange;

	// Price range.
	if (queryWord[0] == '$') {
		int j = 0;
		string price;
		while (j < queryWord.size() && queryWord[j] != '.')
			j++;
		int from = stoi(queryWord.substr(1, j - 1));
		int to = stoi(queryWord.substr(j + 3, queryWord.size() - j - 3));

		// Allow for ascending and descending.
		for (int amount = from; amount <= to; amount += -1 + 2 * (from <= to)) {
			price = '$' + to_string(amount);
			allQueryFromRange.push_back(price);
		}
	}
	// Number range.
	else {
		int j = 0;
		string number;
		while (j < queryWord.size() && queryWord[j] != '.')
			j++;
		int from = stoi(queryWord.substr(0, j - 1));
		int to = stoi(queryWord.substr(j + 3, queryWord.size() - j - 3));
		for (int amount = from; amount <= to; amount += -1 + 2 * (from <= to)) {
			number = to_string(amount);
			allQueryFromRange.push_back(number);
		}
	}

	// Put all document lists of these range tokens into 1 list.
	// Make sure to have duplicates.
	vector<int> allRangeDocumentList;
	unordered_set<int> distinctDocumentList;
	for (string& word : allQueryFromRange) {
		tokens.push_back(word);
		documentList = findDocumentList(root, word);
		if (!documentList.empty()) {
			for (int& doc : documentList) {
				if (distinctDocumentList.find(doc) == distinctDocumentList.end()) {
					allRangeDocumentList.push_back(doc);
					distinctDocumentList.insert(doc);
				}
			}
		}
	}
	sort(allRangeDocumentList.begin(), allRangeDocumentList.end());

	// At least 1 value from range must be in search results. 
	mustHaveDocumentLists.push_back(allRangeDocumentList);
}

// This function reads in and processes a price token from the subquery.
void priceOperator(
	Node*& root,
	vector<string>& tokens, 
	vector<vector<int>>& mustHaveDocumentLists, 
	string& queryWord) {
	extractWord(queryWord);
	vector<int> documentList = findDocumentList(root, queryWord);
	if (!documentList.empty()) {
		tokens.push_back(queryWord);
		mustHaveDocumentLists.push_back(documentList);
	}
}

// This function reads in and processes exact tokens from the subquery.
void exactOperator(
	Node*& root, 
	vector<vector<int>>& mustHaveDocumentLists, 
	vector<string>& tokens, 
	vector<string>& queryTokens, 
	int& currentQueryIndex, int lastIndexOfSubquery) {

	queryTokens[currentQueryIndex] = queryTokens[currentQueryIndex].substr(1, queryTokens[currentQueryIndex].size() - 1);
	// Try to find the ending quotation mark.
	int j = currentQueryIndex;
	while (j <= lastIndexOfSubquery && (queryTokens[j].empty() || queryTokens[j].back() != '"'))
		j++;
	// If we can't find it and the first word is empty, there's no exact tokens.
	// Else we still examine the first word (with the lonely opening qutation mark).
	if (j > lastIndexOfSubquery) {
		if (queryTokens[currentQueryIndex].empty())
			return;
		j = currentQueryIndex;
	}
	
	// Put the exact tokens in a vector.
	vector<string> exactTokens;
	string queryWord;
	for (; currentQueryIndex <= j; ++currentQueryIndex) {
		queryWord = queryTokens[currentQueryIndex];
		if (currentQueryIndex == j && !queryWord.empty() && queryWord.back() == '"')
			queryWord.pop_back();
		if (!queryWord.empty()) {
			extractWord(queryWord);
			tokens.push_back(queryWord);
			exactTokens.push_back(queryWord);
		}
	}

	// Do the ranking before putting into mustHave.
	mustHaveDocumentLists.push_back(exactMatchRanking(root, exactTokens));
}

// This function reads in and processes a synonym token from the subquery.
void synonymOperator(
	Node*& root, 
	vector<vector<int>>& allDocumentLists, 
	vector<string>& tokens, 
	vector<vector<string>>& synonyms, 
	string& queryWord) {

	queryWord = queryWord.substr(1, queryWord.size() - 1);
	extractWord(queryWord);

	// Find all synonyms. If no synonym is found, itself is included in the returned vector.
	vector<string> synonym = getSynonym(queryWord, synonyms);
	vector<int> documentList;
	for (string& synonymWord : synonym) {
		documentList = findDocumentList(root, synonymWord);
		if (!documentList.empty()) {
			tokens.push_back(synonymWord);
			allDocumentLists.push_back(documentList);
		}
	}
}

// This function reads in and processes a normal token from the subquery.
void defaultOperator(
	Node*& root, 
	Node*& stopwordRoot,
	vector<vector<int>>& allDocumentLists, 
	vector<string>& tokens, 
	string& queryWord) {

	extractWord(queryWord);
	tokens.push_back(queryWord);
	if (!isStopword(stopwordRoot, queryWord)) {
		vector<int> documentList = findDocumentList(root, queryWord);
		if (!documentList.empty())
			allDocumentLists.push_back(documentList);
	}
}