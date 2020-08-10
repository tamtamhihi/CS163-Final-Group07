#include "Indexing.h"

// This function returns a new node.
Node* newNode() {
	Node* node = new Node;
	node->isWord = false;
	return node;
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
		if ((c < 0 || c > 255) && MONEY_CHARS.find(c) == string::npos)
			continue;
		keepChar = true;
		// The first character of extracted word should be
		// either an alpha, a digit, a '#' or a money char.
		if (extractedWord == "")
			keepChar = (MONEY_CHARS.find(c) != string::npos) || isalnum(c) || c == '#';
		// Ignore the ',' in words like 19,000.
		if (c == ',' && extractedWord != "" && i < s - 1 && isdigit(extractedWord.back()) && isdigit(word[i + 1]))
			keepChar = false;
		if (keepChar)
			extractedWord += tolower(c);
	}
	while (!extractedWord.empty() && !isalnum(extractedWord.back()))
		extractedWord.pop_back();
	word = extractedWord;
	return !word.empty();
}

// This function loads all stopwords into a trie.
void loadStopwords(Node*& stopwordRoot) {
	ifstream in("stopword.txt");
	if (in) {
		string word;
		while (in >> word) {
			Node* addedWord = addWordReturnFinal(word, stopwordRoot);
			addedWord->isWord = true;
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
	return currentNode->isWord;
}

// This function opens document and read in all words to add to index file.
void inspectDocument(Node*& root, int& documentNum, Node*& stopwordRoot) {
	// Open corresponding file.
	ifstream in(DOCUMENT_PREFIX + to_string(documentNum) + ".txt");
	if (!in)
		return;
	string word;
	// Read in word by word until EOF.
	while (in >> word) {
		// Extract the word and determine if the word should be added.
		if (!extractWord(word))
			continue;
		if (isStopword(stopwordRoot, word))
			continue;
		addWord(word, root, documentNum);
	}
	in.close();
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

// This function traverse a new word and add this document number to the final node.
void addWord(string& word, Node*& root, int& documentNum) {
	Node* addedWord = addWordReturnFinal(word, root);
	// currentNode now points at the final node.
	addedWord->isWord = true;
	int index = lower_bound(addedWord->documentList.begin(), addedWord->documentList.end(), documentNum) 
		-  addedWord->documentList.begin();
	if (index < addedWord->documentList.size() && addedWord->documentList[index] == documentNum)
		return;
	addedWord->documentList.insert(addedWord->documentList.begin() + index, documentNum);
}

// This function traverse the word and add to trie with the given document list.
void addWordWithDocumentList(string& word, Node*& root, vector<int>& documentList) {
	Node* addedWord = addWordReturnFinal(word, root);
	addedWord->isWord = true;
	addedWord->documentList = documentList;
}

// This function print the whole indexed Trie to a file in PreprocessedData/index.txt.
// To make this process more efficient, we use a string to store the word so far.
void printTrieToIndexFile(ofstream& indexFile, Node*& root, string& word) {
	// If the current node is the final node of a word,
	// print out that word together with document list.
	if (root->isWord) {
		//cout << "Writing \"" << word << "\"...\n";
		indexFile << word;
		for (int documentNum : root->documentList)
			indexFile << " " << documentNum;
		indexFile << "\n";
		// We do not return here because there might be more words behind.
		// For example, "and" and "andy".
	}
	// Go through all children and continue adding new characters to the word.
	map<int, Node*>::iterator iterator;
	for (iterator = root->child.begin(); iterator != root->child.end(); ++iterator) {
		word += char(iterator->first);
		printTrieToIndexFile(indexFile, iterator->second, word);
		word.pop_back();
	}
}

// This function cleans up all allocated memory for Trie.
void deleteTrie(Node*& root) {
	map<int, Node*>::iterator iterator;
	for (iterator = root->child.begin(); iterator != root->child.end(); ++iterator)
		deleteTrie(iterator->second);
	delete root;
}

// This function indexes all documents based on the given amount.
void indexing(int numOfDocuments) {
	// Create a root node for main trie.
	Node* root = newNode();
	// Create a root node for stopwords.
	Node* stopwordRoot = newNode();
	loadStopwords(stopwordRoot);

	for (int i = 1; i <= numOfDocuments; ++i)
		inspectDocument(root, i, stopwordRoot);
	cout << "Finished crawling " << numOfDocuments << " documents...\n";

	// Add all words from trie to index file.
	cout << "Adding index to file...\n\n";
	string word = "";
	ofstream indexFile(INDEX_FILE, ios::app);
	printTrieToIndexFile(indexFile, root, word);
	indexFile.close();
	
	// Delete trie from memory.
	deleteTrie(root);
	deleteTrie(stopwordRoot);
}

// This function loads index from file to be ready for searching.
void loadIndex(Node*& root) {
	ifstream indexFile(INDEX_FILE);
	if (!indexFile) {
		cout << "Cannot open file index.\n\n";
		return;
	}
	int documentNum;
	string word, documentLine;
	vector<int> documentList;
	while (indexFile >> word) {
		getline(indexFile, documentLine);
		stringstream docLine(documentLine);
		while (docLine >> documentNum)
			documentList.push_back(documentNum);
		addWordWithDocumentList(word, root, documentList);
		documentList.clear();
	}
	indexFile.close();
}