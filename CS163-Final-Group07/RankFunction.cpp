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

bool findDoc(vector<int> documentList, int doc) {
	for (auto& document : documentList) {
		if (document == doc)
			return true;
	}
	return false;
}

// check if doc is in countDocuments yet
// if yes return true
// else return false
bool docInPair(vector<pair<int, int>> countDocuments, int doc) {
	for (auto& i : countDocuments) {
		if (i.first == doc) {
			// if doc is in countDocuments
			// increase the counter of this doc
			i.second++;
			return true;
		}
	}
	return false;
}

vector<int> orRanking_v2(vector<vector<int>>& allDocuments) {
	if (allDocuments.size() == 0) return vector<int>();

	// if there is just 1 keyword => return all documents that contain it
	if (allDocuments.size() == 1)
		return allDocuments[0];

	// countDocuments has two elements: 
	// 1. the doc num
	// 2. the number of words that this document has
	vector<pair<int, int>> countDocuments;

	for (int i = 0; i < allDocuments.size(); ++i) {
		// for each document, if the current document is not in countDocuments
		// then add it to countDocuments
		for (int j = 0; j < allDocuments[i].size(); ++j) {
			if (!docInPair(countDocuments, allDocuments[i][j])) {
				countDocuments.push_back(pair<int, int>(allDocuments[i][j], 1));
			}
		}
	}

	// Sort the documents in terms of how many different words it contains.
	sort(countDocuments.begin(), countDocuments.end(), sortDecreasingSecondElement);
	int totalDocuments = min(10, (int)countDocuments.size());

	vector<int> result(totalDocuments);
	for (int i = 0; i < totalDocuments; ++i)
		result[i] = countDocuments[i].first;
	return result;
}


vector<int> andRanking(vector<vector<int>>& allDocuments) {
	if (allDocuments.size() == 0) return vector<int>();

	// if there is just 1 keyword => return all documents that contain it
	if (allDocuments.size() == 1)
		return allDocuments[0];

	vector<int> countDocuments;
	int totalDocuments = 0;

	// loop through all the documents index that contain first keyword
	for (int i = 0; i < allDocuments[0].size(); ++i) {
		// for each document index
		// check if other keywords is in that document
		// by looping through other lists of words
		for (int j = 1; j < allDocuments.size(); ++j) {
			// if we don't find the current keyword is in the document
			// which means we don't add this document index
			if (!findDoc(allDocuments[j], allDocuments[0][i])) {
				break;
			}
			// if all keywords present in the current document
			// add it to countDocuments
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

vector<int> negateRanking(vector<vector<int>> allDocuments) {
	if (allDocuments.size() == 0) return vector<int>();

	// if there is just 1 keyword => return all documents that contain it
	if (allDocuments.size() == 1)
		return allDocuments[0];

	vector<int> countDocuments;

	for (int i = 0; i < allDocuments[0].size(); ++i) {
		// for each document index
		// check if other keywords is in that document
		// by looping through other lists of words
		countDocuments.push_back(allDocuments[0][i]);

		for (int j = 1; j < allDocuments.size(); ++j) {
			// if we don't find the current keyword is in the document
			// which means we don't add this document index
			if (findDoc(allDocuments[j], allDocuments[0][i])) {
				countDocuments.pop_back();	
				break;
			}
		}
	}

	int resultSize = min(SEARCH_CAPACITY, (int)countDocuments.size());
	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i) {
		result[i] = countDocuments[i];
	}
	return result;
}