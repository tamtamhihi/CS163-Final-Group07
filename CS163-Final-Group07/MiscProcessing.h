#ifndef _MISCPROCESSING_H_
#define _MISCPROCESSING_H_

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

const int MAXIMUM_FILE = 12000;
const int SEARCH_CAPACITY = 10;
const string DOCUMENT_PREFIX = "ProjectData/";

// A node designed to be in a Trie structure.
struct Node {
	map<int, Node*> child;
	int isWord;
	vector<int> documentList;
};

Node* newNode();
void moveCurrentNode(Node*& currentNode, char& c);
Node* addWordReturnFinal(string& word, Node*& root);
void deleteTrie(Node*& root);
bool isDigit(char& c);
bool extractWord(string& word);
void loadStopwords(Node*& stopwordRoot);
bool isStopword(Node*& stopwordRoot, string& word);
vector<int> findDocumentList(Node*& root, string& word);
bool isDocumentInList(vector<int>& documentList, int documentNum);
bool sortDecreasingSecondElement(const pair<int, int>& a, const pair<int, int>& b);
void stringlower(string& word);

#endif // !_MISCPROCESSING_H_