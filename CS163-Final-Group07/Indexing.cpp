#include "Indexing.h"

// This function returns a new node.
Node* newNode() {
	Node* node = new Node;
	node->isWord = false;
	return node;
}

// This function opens document.
void inspectDocument(Node*& root, int& documentNum) {
	// Open corresponding file.
	ifstream in(DOCUMENT_PREFIX + to_string(documentNum) + ".txt");
	if (!in)
		return;
	string word;
	// Read in word by word until EOF.
	while (in >> word) {
		// Lowercase the word.
		for (char& c : word)
			c = tolower(c);
		// If the word is a question mark, we ignore it.
		if (QUESTION_MARKS.find(word) != string::npos)
			continue;
		// TODO: Ignore stop words.
		addWord(word, root, documentNum);
	}
	in.close();
}

// This function traverse a new word and add this document number to the final node.
void addWord(string& word, Node*& root, int& documentNum) {
	int s = word.size(), asciiMark;
	Node* currentNode = root;
	for (int i = 0; i < s; ++i) {
		// If the last character is a question mark, we might ignore it.
		if (i == s-1 && QUESTION_MARKS.find(word[i]) != string::npos)
			break;
		asciiMark = int(word[i]);
		// If the corresponding child of current node does not exist,
		// we create a new one.
		if (currentNode->child.find(asciiMark) == currentNode->child.end())
			currentNode->child[asciiMark] = newNode();
		currentNode = currentNode->child[asciiMark];
	}
	// currentNode now points at the final node.
	currentNode->isWord = true;
	currentNode->documentList.push_back(documentNum);
}

// This function print the whole indexed Trie to a file in PreprocessedData/index.txt.
// To make this process more efficient, we use a string to store the word so far.
void printTrieToIndexFile(Node*& root, string& word) {
	// If the current node is the final node of a word,
	// print out that word together with document list.
	if (root->isWord) {
		ofstream out(INDEX_FILE, ios::app);
		if (out) {
			out << word;
			for (int documentNum : root->documentList)
				out << " " << documentNum;
			out << "\n";
			out.close();
		}
		// We do not return here because there might be more words behind.
		// For example, "and" and "andy".
	}
	// Go through all children and continue adding new characters to the word.
	map<int, Node*>::iterator iterator;
	for (iterator = root->child.begin(); iterator != root->child.end(); ++iterator) {
		word += char(iterator->first);
		printTrieToIndexFile(iterator->second, word);
		word.pop_back();
	}
}

// This function cleans up all allocated memory for Trie.
void deleteTrie(Node*& root) {
	map<int, Node*>::iterator iterator;
	for (iterator = root->child.begin(); iterator != root->child.end(); ++iterator) {
		deleteTrie(iterator->second);
	}
	delete root;
}

// This function indexes all documents based on the given amount.
void indexing(int numOfDocuments) {
	Node* root = newNode();
	for (int i = 1; i <= numOfDocuments; ++i)
		inspectDocument(root, i);
	string word = "";
	printTrieToIndexFile(root, word);
	deleteTrie(root);
}