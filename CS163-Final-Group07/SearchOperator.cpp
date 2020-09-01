#include "SearchOperator.h"

void orOperator(vector<vector<int>>& allDocumentLists, vector<string>& tokens) {
	vector<int> searchResult = orRanking(allDocumentLists);
	cout << "Top " << searchResult.size() << " results:\n\n";
	for (int document : searchResult) {
		displayResult(document, tokens);
	}
}

void andOperator(vector<vector<int>>& allDocumentLists, vector<string>& tokens) {
	vector<int> searchResult = andRanking(allDocumentLists);
	cout << "Top " << searchResult.size() << " results:\n\n";
	for (int document : searchResult) {
		displayResult(document, tokens);
	}
}

void negateOperator(vector<vector<int>>& allDocumentLists, vector<vector<int>>& negateDocumentLists, vector<string>& tokens) {
	vector<int> searchResult = negateRanking(allDocumentLists, negateDocumentLists);
	cout << "Top " << searchResult.size() << " results:\n\n";
	for (int document : searchResult) {
		displayResult(document, tokens);
	}
}