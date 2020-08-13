#include "Indexing.h"

// This function opens document and read in all words to add to index file.
void inspectDocument(Node*& root, string& documentPath, int& documentNum, Node*& stopwordRoot) {
	// Open corresponding file.
	ifstream in(DOCUMENT_PREFIX + documentPath);
	if (!in)
		return;
	string word;
	// Read in word by word until EOF.
	while (in >> word) {
		// Extract the word and determine if the word should be added.
		if (!extractWord(word) || isStopword(stopwordRoot, word))
			continue;
		addWordWithDocumentNum(word, root, documentNum);
	}
	in.close();
}

// This function traverse a new word and add this document number to the final node.
void addWordWithDocumentNum(string& word, Node*& root, int& documentNum) {
	Node* addedWord = addWordReturnFinal(word, root);
	// currentNode now points at the final node.
	addedWord->isWord = true;
	if (!addedWord->documentList.empty() && addedWord->documentList.back() == documentNum)
		return;
	addedWord->documentList.push_back(documentNum);
}

// This function traverse the word and add to trie with the given document list.
void addWordWithDocumentList(string& word, Node*& root, vector<int>& documentList) {
	Node* addedWord = addWordReturnFinal(word, root);
	addedWord->isWord = true;
	addedWord->documentList = documentList;
}

// This function print the whole indexed Trie to a file in PreprocessedData/index.txt.
// To make this process more efficient, we use a string to store the word so far.
void printIndexToFile(ofstream& indexFile, Node*& root, string& word) {
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
		printIndexToFile(indexFile, iterator->second, word);
		word.pop_back();
	}
}

// This function indexes all documents based on the given amount.
void indexing() {
	// Create a root node for main trie.
	Node* root = newNode();
	// Create a root node for stopwords.
	Node* stopwordRoot = newNode();
	loadStopwords(stopwordRoot);
	ifstream allDocuments(DOCUMENT_PREFIX + "___index.txt");
	if (allDocuments) {
		string documentPath;
		int documentCount = 0;
		while (getline(allDocuments, documentPath)) {
			documentCount++;
			if (documentCount % 200 == 0)
				cout << "Crawled " << documentCount << "docs...\n";
			inspectDocument(root, documentPath, documentCount, stopwordRoot);
		}
		cout << "Finished crawling " << documentCount << " documents...\n";

		// Add all words from trie to index file.
		cout << "Adding index to file...\n\n";
		string word = "";
		ofstream indexFile(INDEX_FILE, ios::app);
		printIndexToFile(indexFile, root, word);
		indexFile.close();
		allDocuments.close();
	}
	// Delete trie from memory.
	deleteTrie(root);
	deleteTrie(stopwordRoot);
}

// This function retrieve data from "index.txt" into a Trie to be ready for searching.
void retrieve(Node*& root) {
	// Open index file.
	ifstream in(INDEX_FILE);
	if (!in)
		return;
	cout << "\t\t\t\t\tRetrieving index...\n";

	string line, token;
	vector<int> documents;
	int documentNum = 0;

	// Read in line by line until EOF.
	while (getline(in, line)) {
		// Split the line into tokens
		// First token will hold the keyword
		// others will hold the document number
		stringstream tokenLine(line);
		tokenLine >> token;
		documents.clear();
		while (tokenLine >> documentNum)
			documents.push_back(documentNum);
		// Add word to root.
		addWordWithDocumentList(token, root, documents);
	}
	cout << "\t\t\t\tFinish retrieving index...\n\n";
	in.close();
}