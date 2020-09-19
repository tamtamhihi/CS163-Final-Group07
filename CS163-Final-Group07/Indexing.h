#ifndef _INDEXING_H_
#define _INDEXING_H_

#include <iostream>
#include <ctype.h>
#include <unordered_map>
#include "MiscProcessing.h"
using namespace std;

const string INDEX_FILE = "PreprocessedData/index.txt";
const string DOCUMENTPATH_FILE = "ProjectData/___index.txt";
const string FILETYPE_FILE = "PreprocessedData/filetype.txt";
const string PROPS_FILE = "PreprocessedData/props.txt";
const string DOCNUM_FILE = "PreprocessedData/numdoc.txt";
const string INTITLE_FILE = "PreprocessedData/intitle.txt";

void addNewDocumentsToIndex(
	Node*& root,
	unordered_map<string, vector<int>>& intitleRoot,
	Node*& stopwordRoot,
	unordered_map<string, vector<int>>& filetypes,
	string& filepath);
void addWordWithDocumentNum(
	string& word, 
	Node*& root, 
	int& documentNum
);
void addWordWithDocumentList(
	string& word,
	Node*& root, 
	vector<int>& documentList
);
void changeTotalDocumentNum(int newDocumentNum);
void indexing();
void indexingUnorderedMap();
void inspectDocument(
	Node*& root,
	unordered_map<string, vector<int>>& intitleRoot,
	string& documentPath,
	int& documentNum,
	Node*& stopwordRoot
);
void inspectDocumentUnorderedMap(
	unordered_map<string, vector<int>>& root,
	unordered_map<string, vector<int>>& intitleRoot,
	string& documentPath,
	int& documentNum,
	Node*& stopwordRoot
);
void printIndexToFile(
	ofstream& indexFile, 
	Node*& root, 
	string& word
);
void printIndexToFileUnorderedMap(
	ofstream& indexFile, 
	unordered_map<string, 
	vector<int>>& root
);
void printFiletypeUnorderedMap(
	unordered_map<string, vector<int>>& filetypes
);
void printIntitleUnorderedMap(
	unordered_map<string, vector<int>>& intitleRoot
);
void preprocessSynonyms();
string getFiletypeFromFilepath(string& documentPath);
void removeNullFiles();
void retrieve(Node*& root);
void retrieveUnorderedMap(unordered_map<string, vector<int>>& root);
void loadFiletypes(unordered_map<string, vector<int>>& filetypes);
int getTotalDocumentNum();
void addNewDocumentName(string& filename, int currentTotalDocumentNum);
void copyFromInToOut(ifstream& in, ofstream& out);
string getFilenameFromPath(string& filepath);

#endif