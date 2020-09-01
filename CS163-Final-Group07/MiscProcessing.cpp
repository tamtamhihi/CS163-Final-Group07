#include "MiscProcessing.h"

// This function returns a new node.
Node* newNode() {
	Node* node = new Node;
	node->isWord = -1;
	return node;
}

void moveCurrentNode(Node*& currentNode, char& c) {
	if (currentNode->child.find(int(c)) == currentNode->child.end())
		currentNode->child[int(c)] = newNode();
	currentNode = currentNode->child[int(c)];
}

// This function adds a word to trie of index and return the node of the final character.
Node* addWordReturnFinal(string& word, Node*& root) {
	int s = word.size(), asciiMark;
	Node* currentNode = root;
	for (int i = 0; i < s; ++i) {
		asciiMark = int(word[i]);
		// If the corresponding child of current node does not exist,
		// we create a new one.
		if (currentNode->child.find(asciiMark) == currentNode->child.end())
			currentNode->child[asciiMark] = newNode();
		currentNode = currentNode->child[asciiMark];
	}
	return currentNode;
}

// This function cleans up all allocated memory for Trie.
void deleteTrie(Node*& root) {
	map<int, Node*>::iterator iterator;
	for (iterator = root->child.begin(); iterator != root->child.end(); ++iterator)
		deleteTrie(iterator->second);
	delete root;
}

// This function checks if a given character is a digit (0-9).
bool isDigit(char& c) {
	return (c - '0' >= 0 && c - '0' <= 9);
}

// This function extract a correct word from a string,
// including removing unnecessary leading or trailing characters.
// It returns true if the word is successfully extracted
// and false if the word should not be added at all.
bool extractWord(string& word) {
	// 1-character word should only be added if either
	// an alpha or a digit.
	if (word.size() == 1 && (word[0] < 0 || word[0] > 255 || !isalnum(word[0])))
		return false;
	string extractedWord = "";
	bool keepChar;
	char c;
	int s = word.size();
	for (int i = 0; i < s; ++i) {
		c = word[i];
		// Skipping abnormal charaters.
		if (c == ' ' || c < 0 || c > 255)
			continue;
		keepChar = true;
		// The first character of extracted word should be
		// either an alpha, a digit, a '#' or a money char.
		if (extractedWord == "")
			keepChar = (c == 35) || isalnum(c) || (c == 36);
		// Ignore the ',' in words like 19,000.
		if (c == ',' && extractedWord != "" && i < s - 1 && isDigit(extractedWord.back()) && isDigit(word[i + 1]))
			keepChar = false;
		if (keepChar) {
			if (isalpha(c))
				extractedWord += tolower(c);
			else
				extractedWord += c;
		}
	}
	while (!extractedWord.empty() && !isalnum(extractedWord.back()) && c != '+')
		extractedWord.pop_back();
	word = extractedWord;
	return !word.empty();
}

// This function loads all stopwords into a trie.
void loadStopwords(Node*& stopwordRoot) {
	ifstream in("stopwords.txt");
	if (in) {
		string word;
		while (in >> word) {
			Node* addedWord = addWordReturnFinal(word, stopwordRoot);
			addedWord->isWord = 1;
		}
		in.close();
	}
}

// This function checks if the given is existent in the stopword's trie.
bool isStopword(Node*& stopwordRoot, string& word) {
	int ascciMark, s = word.size();
	Node* currentNode = stopwordRoot;
	for (int i = 0; i < s; ++i) {
		ascciMark = int(word[i]);
		if (currentNode->child.find(ascciMark) == currentNode->child.end())
			return false;
		currentNode = currentNode->child[ascciMark];
	}
	// currentNode now points to the final character's node.
	// If this is the end of a word, then the given word is a stopword.
	return (currentNode->isWord != -1);
}

// This function finds a word from index trie and returns
// the corresponding document list. If the word is not found,
// an empty vector is returned.
vector<int> findDocumentList(Node*& root, string& word) {
	Node* currentNode = root;
	int ascciMark;
	vector<int> documentList;
	for (char c : word) {
		ascciMark = int(c);
		if (currentNode->child.find(ascciMark) == currentNode->child.end())
			return documentList;
		currentNode = currentNode->child[ascciMark];
	}
	if (currentNode->isWord != -1)
		return currentNode->documentList;
	return documentList;
}

// This function finds if a document num exists in a document list.
bool isDocumentInList(vector<int>& documentList, int documentNum) {
	int index = lower_bound(documentList.begin(), documentList.end(), documentNum) - documentList.begin();
	if (index >= 0 && index < documentList.size() && documentList[index] == documentNum)
		return true;
	return false;
}

// This bool function is used to be a callback to sort the list of documents
// in decreasing order of different words it contains.
bool sortDecreasingSecondElement(const pair<int, int>& a, const pair<int, int>& b) {
	return a.second > b.second;
}

// This function lowers all characters in a string.
void stringlower(string& word) {
	for (auto& c : word)
		c = tolower(c);
}