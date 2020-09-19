#include "SearchSubroutine.h"

// This function examines a single word
// and extract operator (if exists).
int filter(string& word) {
	int oper = 0;
	if (word == "and")
		oper = AND;
	else if (word == "or")
		oper = OR;
	else if (word[0] == '-')
		oper = NEGATE;
	else if (word.size() >= 8 && word.substr(0, 8) == "intitle:")
		oper = INTITLE;
	else if (word.size() >= 9 && word.substr(0, 9) == "filetype:")
		oper = FILETYPE;
	else if (word[0] == '$') {
		oper = PRICE;
		int i = 1, s = word.size();
		while (i < s && isdigit(word[i]))
			i++;
		if (i < word.size() - 3 && word[i] == '.' && word[i + 1] == '.' && word[i + 2] == '$') {
			for (int j = i + 3; j < s; ++j) {
				if (!isdigit(word[j]))
					break;
				if (j == s - 1)
					oper = RANGE;
			}
		}
	}
	else if (word[0] == '"')
		oper = EXACT;
	else if (word[0] == '~')
		oper = SYNONYM;
	else {
		int i = 0;
		while (i < word.size() && isdigit(word[i]))
			i++;
		if (i < word.size() - 2 && word[i] == '.' && word[i+1] == '.') {
			while (i < word.size() && isdigit(word[i]))
				i++;
			if (i == word.size())
				oper = RANGE;
		}
	}
	return oper;
}

// This function gets the document filename from ___index.txt based on documentNum.
string getDocumentPath(int documentNum) {
	ifstream in(DOCUMENTPATH_FILE);
	if (in) {
		string path;
		for (int i = 0; i < documentNum; ++i)
			getline(in, path);
		in.close();
		return path;
	}
}

// This function gets all synonyms of the queryWord by finding it in
// one of the list in synonyms. If it's found no where, the returned
// list contains only itself.
vector<string> getSynonym(string& queryWord, vector<vector<string>>& synonyms) {
	int S = synonyms.size();

	// All lists in synonyms have been sorted in preprocessSynonyms,
	// so we can use lower_bound to find the token faster.
	for (int i = 0; i < S; ++i) {
		if (isTokenInList(queryWord, synonyms[i]) != -1)
			return vector<string>(synonyms[i]);
	}

	// Found nowhere, we return only that word.
	vector<string> emptyResults;
	emptyResults.push_back(queryWord);
	return emptyResults;
}

vector<int> andMatch(Node*& root, vector<string>& tokens) {
	if (tokens.empty())
		return vector<int>();
	vector<int> docList = findDocumentList(root, tokens[0]), searchResult, temporaryDocList;
	int s = docList.size(), S = tokens.size();
	for (int i = 0; i < s; ++i) {
		bool chosen = true;
		for (int j = 1; j < S; ++j) {
			temporaryDocList = findDocumentList(root, tokens[j]);
			if (!isDocumentInList(temporaryDocList, docList[i])) {
				chosen = false;
				break;
			}
		}
		if (chosen)
			searchResult.push_back(docList[i]);
	}
	return searchResult;
}

vector<int> findExactMatchPosition(int documentNum, vector<string>& tokens) {
	string path = getDocumentPath(documentNum);
	ifstream fileStream(DOCUMENT_PREFIX + path);
	vector<int> result;
	if (fileStream) {
		string word;
		int count = 0, s = tokens.size();
		while (fileStream >> word) {
			extractWord(word);
			if (word == tokens[0]) {
				bool isExactMatch = true;
				for (int i = 1; i < s; ++i) {
					fileStream >> word;
					stringlower(word);
					if (word != tokens[i]) {
						isExactMatch = false;
						break;
					}
				}
				if (isExactMatch)
					result.push_back(count);
				count += tokens.size();
			}
			else
				count++;
		}
		fileStream.close();
	}
	return result;
}

// This function splits the query into tokens in queryTokens,
// finds all "and", "or" keywords' positions into andOrPositions.
void findAndOr(string& query, vector<string>& queryTokens, vector<int>& andOrPositions) {
	stringstream queryStream(query);
	string word;
	int queryWordCount = 0;

	// Read in each word. We don't extract word here
	// to prevent losing operators.
	while (queryStream >> word) {
		queryTokens.push_back(word);
		if (word == "and" || word == "or")
			andOrPositions.push_back(queryWordCount);
		++queryWordCount;
	}
}

// Find if a token is in a list. The list must be sorted beforehand.
int isTokenInList(string& token, vector<string>& tokens) {
	int index = lower_bound(tokens.begin(), tokens.end(), token) - tokens.begin();
	if (index < (int)tokens.size() && tokens[index] == token)
		return index;
	return -1;
}