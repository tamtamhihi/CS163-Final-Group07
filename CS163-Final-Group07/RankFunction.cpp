#include "RankFunction.h"
#include "MiscProcessing.h"

// This function takes in a lot of documents, each of which 
// containing a different word in the query. We sort the documents
// in terms of how many diffrent words they contain and return top 5.
vector<int> orRanking(vector<vector<int>>& allDocuments) {
	// Each pair contains the number of document and the
	// number of different words it contains.
	vector<pair<int, int>> countDocuments(MAXIMUM_FILE);
	for (int i = 1; i < MAXIMUM_FILE; ++i)
		countDocuments[i] = make_pair(i, 0);

	int s = allDocuments.size(), totalDocuments = 0;
	for (int i = 0; i < s; ++i) {
		totalDocuments += allDocuments[i].size();
		for (int document : allDocuments[i])
			countDocuments[document].second++;
	}
	
	// Sort the documents in terms of how many different words it contains.
	sort(countDocuments.begin(), countDocuments.end(), sortDecreasingSecondElement);
	
	int resultSize = min(SEARCH_CAPACITY, totalDocuments);
	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i)
		result[i] = countDocuments[i].first;
	return result;
}

bool findDoc(vector<int> word, int doc) {
	for (auto& document : word) {
		if (document == doc)
			return true;
	}
	return false;
}

vector<int> andRanking(vector<vector<int>>& allDocuments) {
	vector<int> countDocuments;
	vector<bool> present(allDocuments[0].size(), true);
	int totalDocuments = 0;

	for (int i = 0; i < allDocuments[0].size(); ++i) {
		
		for (int j = 1; j < allDocuments.size(); ++j) {
			if (!findDoc(allDocuments[j], allDocuments[0][i])) {
				break;
			}
			if (j == allDocuments.size() - 1)
				countDocuments.push_back(allDocuments[0][i]),
				totalDocuments++;
		}
	}
	int resultSize = min(SEARCH_CAPACITY, totalDocuments);
	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i) {
		result[i] = countDocuments[i];
	}
	return result;
}