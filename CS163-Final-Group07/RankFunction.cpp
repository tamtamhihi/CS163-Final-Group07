#include "RankFunction.h"

// This function takes in a lot of documents, each of which 
// containing a different word in the query. We keep 
vector<int> andRanking(vector<vector<int>>& allDocuments) {
	if (allDocuments.size() == 0) return vector<int>();

	vector<int> andResults;
	int totalDocuments = 0;
	int firstToken = allDocuments[0].size();
	int allToken = allDocuments.size();

	// Loop through all the documents index that contain
	// the first keyword, keep in to results if it contains
	// all other keywords.
	for (int i = 0; i < firstToken; ++i) {
		// Check if other keywords is in that document
		// by looping through other lists of words.
		bool chosen = true;
		for (int j = 1; j < allToken; ++j) {
			// If we don't find the current keyword in the document,
			// we don't add this document index.
			if (!isDocumentInList(allDocuments[j], allDocuments[0][i])) {
				chosen = false;
				break;
			}
		}
		// If all keywords present in the current document
		// add it to results.
		if (chosen) {
			andResults.push_back(allDocuments[0][i]);
			totalDocuments++;
			if (totalDocuments == SEARCH_CAPACITY)
				break;
		}
	}
	return andResults;
}

// This function takes in a lot of documents, each of which 
// containing a different word in the query. We sort the documents
// in terms of how many diffrent words they contain and return top 5.
vector<int> orRanking(vector<vector<int>>& allDocuments, bool keepOriginal) {
	if (allDocuments.size() == 0) return vector<int>();

	// Each pair contains the number of document and the
	// number of different words it contains.
	vector<pair<int, int>> countDocuments(MAXIMUM_FILE);
	for (int i = 1; i < MAXIMUM_FILE; ++i)
		countDocuments[i] = make_pair(i, 0);

	int s = allDocuments.size(), totalDocuments = 0;
	for (int i = 0; i < s; ++i) {
		for (int document : allDocuments[i]) {
			countDocuments[document].second++;
			// Count distinct documents containing at least 1 token
			if (countDocuments[document].second == 1)
				totalDocuments++;
		}
	}
	
	// Sort the documents in terms of how many different words it contains.
	sort(countDocuments.begin(), countDocuments.end(), sortDecreasingSecondElement);

	int resultSize = min(SEARCH_CAPACITY, totalDocuments);
	// Do not resize search results to use this as a 
	// subroutine for other search operator.
	if (keepOriginal)
		resultSize = totalDocuments;

	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i)
		result[i] = countDocuments[i].first;
	return result;
}

vector<int> negateRanking(vector<vector<int>>& allDocuments, vector<vector<int>>& negateDocuments) {
	if (allDocuments.size() == 0) return vector<int>();

	// Find all documents that containing most searched tokens.
	vector<int> temporaryResults = orRanking(allDocuments, true);

	vector<int> negateResults;

	// Negating tokens.
	for (int doc : temporaryResults) {
		bool chosen = true;
		for (auto& negateDocumentList : negateDocuments) {
			if (isDocumentInList(negateDocumentList, doc)) {
				chosen = false;
				break;
			}
		}
		if (chosen)
			negateResults.push_back(doc);
	}

	int resultSize = min(SEARCH_CAPACITY, (int)negateResults.size());
	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i)
		result[i] = negateResults[i];
	return result;
}