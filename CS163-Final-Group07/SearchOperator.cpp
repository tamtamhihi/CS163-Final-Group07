#include "SearchOperator.h"

void search() {
	Node* root = newNode();
	retrieve(root);

	string query;
	cout << "Search File: ";
	getline(cin, query);

	operatorAnd(root, query);
}

// This function retrieve data from "index.txt" and create a Trie
void retrieve(Node*& root) {
	// Open corresponding file.
	ifstream in("PreprocessedData/index.txt");
	if (!in)
		return;

	string line, delimiter = " ";
	vector<string> tokens;
	size_t pos = 0;
	Node* currentNode = root;
	int asciiMark;

	// Read in line by line until EOF.
	while (getline(in, line)) {
		// split the line into tokens
		// tokens[0] will hold the keyword
		// others will hold the document number
		while ((pos = line.find(delimiter)) != std::string::npos) {
			tokens.push_back(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());
		}
		tokens.push_back(line);

		// create Trie for the keyword
		for (char& c : tokens[0]) {
			asciiMark = int(c);
			if (currentNode->child.find(asciiMark) == currentNode->child.end())
				currentNode->child[asciiMark] = newNode();
			currentNode = currentNode->child[asciiMark];
		}
		currentNode->isWord = true;

		for (int i = 1; i < tokens.size(); ++i)
			currentNode->documentList.push_back(stoi(tokens[i]));

		tokens.clear();
	}

	in.close();
}

bool operatorAnd(Node* root, string word) {
	int size = word.length(), asciiMark;

	for (int i = 0; i < size; ++i) {
		asciiMark = int(word[i]);

		// if we can't find asciiMark then return false
		if (root->child.find(asciiMark) == root->child.end())
			return false;

		// if this is the last character of word but it is not a word in root
		// then return false
		if (i == size - 1 && !root->child[asciiMark]->isWord)
			return false;

		root = root->child[asciiMark];
	}

	// if it is a word then print all the document id
	for (auto i : root->documentList)
		cout << i << " ";
	cout << "\n";

	return true;
}
