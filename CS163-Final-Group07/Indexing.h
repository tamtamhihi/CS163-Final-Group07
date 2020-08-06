#ifndef _INDEXING_H_
#define _INDEXING_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctype.h>
using namespace std;

const string DOCUMENT_PREFIX = "ProjectData/";
const string INDEX_FILE = "PreprocessedData/index.txt";
const string QUESTION_MARKS = ",.?!";

// Create a Trie structure to process all words in documents from ProjectData/.
struct Node {
	map<int, Node*> child;
	bool isWord;
	vector<int> documentList;
};

Node* newNode();
void inspectDocument(Node*& root, int& documentNum);
void addWord(string& word, Node*& root, int& documentNum);
void printTrieToIndexFile(Node*& root, string& word);
void deleteTrie(Node*& root);
void indexing(int numOfDocuments);

#endif