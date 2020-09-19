#pragma once
#include "Indexing.h"
#include "SearchOperator.h"
#include "History.h"
#include "gui.h"
#include "SearchSubroutine.h"
#include "SearchResult.h"
#include <conio.h>
#include <time.h>
#include <chrono>

#define ALPHA  -32
#define UP      72
#define DOWN    80
#define LEFT    75
#define RIGHT   77

const int MAXIMUM_DISPLAYED_SEARCH_RESULTS = 10;

void initiateSearchEngine();

void displayGuides();

void addDocument(
	Node*& root,
	unordered_map<string, vector<int>>& intitleRoot,
	Node*& stopwordRoot,
	unordered_map<string,
	vector<int>>&filetypes
);

void search(
	Node*& root, 
	unordered_map<string, vector<int>>& intitleRoot,
	Node*& stopwordRoot, 
	Node*& history, 
	vector<string>& logs, 
	vector<vector<string>>& synonyms,
	unordered_map<string, vector<int>>& filetypes
);

void getQuery(
	string& query, 
	Node*& history, 
	vector<string>& logs
);

void processSubquery(
	Node*& root,
	Node*& stopwordRoot,
	vector<vector<string>>& synonyms,
	vector<string>& intitleTokens,
	vector<string>& filetypes,
	vector<string>& tokens,
	vector<int>& resultDocuments,
	vector<string>& query,
	int left,
	int right
);