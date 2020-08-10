#ifndef _INDEXING_H_
#define _INDEXING_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <ctype.h>
#include <algorithm>
using namespace std;

const string DOCUMENT_PREFIX = "ProjectData/";
const string INDEX_FILE = "PreprocessedData/index.txt";
const string MONEY_CHARS = "$£€";
const int MAX_CHILD = 100;

// A Trie structure to process all words in documents from ProjectData/.
struct Node {
	map<int, Node*> child;
	bool isWord;
	vector<int> documentList;
};

Node* newNode();
bool extractWord(string& word);
void loadStopwords(Node*& stopwordRoot);
bool isStopword(Node*& stopwordRoot, string& word);
void inspectDocument(Node*& root, int& documentNum, Node*& stopwordRoot);
Node* addWordReturnFinal(string& word, Node*& root);
void addWord(string& word, Node*& root, int& documentNum);
void addWordWithDocumentList(string& word, Node*& root, vector<int>& documentList);
void printTrieToIndexFile(ofstream& indexFile, Node*& root, string& word);
void deleteTrie(Node*& root);
void indexing(int numOfDocuments);
void loadIndex(Node*& root);

#endif