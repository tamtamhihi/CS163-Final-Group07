#include "RankFunction.h"
#include "MiscProcessing.h"

// This function takes in a lot of documents, each of which 
// containing a different word in the query. We sort the documents
// in terms of how many diffrent words they contain and return top 5.
vector<int> andRanking(vector<vector<int>>& allDocuments) {
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