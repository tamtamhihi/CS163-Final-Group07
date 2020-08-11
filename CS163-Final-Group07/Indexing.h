#ifndef _INDEXING_H_
#define _INDEXING_H_

#include <iostream>
#include <ctype.h>
#include "MiscProcessing.h"
using namespace std;

const string INDEX_FILE = "PreprocessedData/index.txt";

void inspectDocument(Node*& root, string& documentPath, int& documentNum, Node*& stopwordRoot);
void addWordWithDocumentNum(string& word, Node*& root, int& documentNum);
void addWordWithDocumentList(string& word, Node*& root, vector<int>& documentList);
void printIndexToFile(ofstream& indexFile, Node*& root, string& word);
void indexing();
void retrieve(Node*& root);

#endif