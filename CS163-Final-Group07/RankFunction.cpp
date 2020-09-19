#include "RankFunction.h"

// This function only keeps documents that appear in all lists of documents.
vector<int> andRanking(vector<vector<int>>& allDocumentLists) {
	if (allDocumentLists.empty())
		return vector<int>();
	vector<int> searchResult;
	for (int i = 0; i < (int)allDocumentLists[0].size(); ++i) {
		bool chosen = true;
		for (int j = 1; j < (int)allDocumentLists.size(); ++j) {
			if (!isDocumentInList(allDocumentLists[j], allDocumentLists[0][i])) {
				chosen = false;
				break;
			}
		}
		if (chosen)
			searchResult.push_back(allDocumentLists[0][i]);
	}
	return searchResult;
}

// This function ranks the documents in terms of how many different
// lists of documents it appears in.
vector<int> orRanking(vector<vector<int>>& allDocumentLists) {
	vector<pair<int, int>> countDocuments(MAXIMUM_FILE);
	for (int i = 1; i < MAXIMUM_FILE; ++i)
		countDocuments[i] = make_pair(i, 0);
	int s = allDocumentLists.size(), documentCount = 0;
	for (int i = 0; i < s; ++i) {
		for (int document : allDocumentLists[i]) {
			if (!countDocuments[document].second)
				documentCount++;
			countDocuments[document].second++;
		}
	}
	// Sort the documents in terms of how many different words it contains.
	sort(countDocuments.begin(), countDocuments.end(), sortDecreasingSecondElement);
	vector<int> result(documentCount);
	for (int i = 0; i < documentCount; ++i)
		result[i] = countDocuments[i].first;
	return result;
}

//
vector<int> negateRanking(vector<int>& documentList, vector<vector<int>>& negateDocumentLists) {
	vector<int> searchResult;
	for (int i = 0; i < (int)documentList.size(); ++i) {
		bool chosen = true;
		for (int j = 0; j < (int)negateDocumentLists.size(); ++j) {
			if (isDocumentInList(negateDocumentLists[j], documentList[i])) {
				chosen = false;
				break;
			}
		}
		if (chosen)
			searchResult.push_back(documentList[i]);
	}
	return searchResult;
}

vector<int> andOrRanking(vector<int>& mustHave, vector<int>& optional) {
	if (mustHave.empty())
		return vector<int>(optional);
	int topIndexInMust = 0;
	for (int i = 0; i < (int)optional.size(); ++i) {
		if (isDocumentInList(mustHave, optional[i])) {
			swap(mustHave[topIndexInMust++], mustHave[lower_bound(mustHave.begin(), mustHave.end(), optional[i]) - mustHave.begin()]);
		}
	}
	return vector<int>(mustHave);
}

vector<int> exactMatchRanking(Node*& root, vector<string>& exactTokens) {
	vector<int> andMatchResult = andMatch(root, exactTokens), exactMatchPosition;
	int S = andMatchResult.size();
	vector<pair<int, int>> temporaryResult(S);
	for (int i = 0; i < S; ++i) {
		exactMatchPosition = findExactMatchPosition(andMatchResult[i], exactTokens);
		temporaryResult[i] = make_pair(andMatchResult[i], exactMatchPosition.size());
	}
	sort(temporaryResult.begin(), temporaryResult.end(), sortDecreasingSecondElement);
	vector<int> searchResult;
	for (int i = 0; i < S; ++i) {
		if (temporaryResult[i].second)
			searchResult.push_back(temporaryResult[i].first);
		else
			break;
	}
	return searchResult;
}

vector<int> wildcardRanking(Node*& root, vector<string>& tokens, vector<string>& queryTokens, int left, int right) {
	vector<int> wildPositions;
	for (int i = left; i <= right; ++i)
		if (queryTokens[i] == "*")
			wildPositions.push_back(i);
	wildPositions.push_back(right + 1);
	vector<string> rightTokens, leftTokens, allTokens;
	vector<int> pos;
	vector<vector<string>> splitTokens;
	for (int j = left; j < wildPositions[0]; ++j) {
		allTokens.push_back(queryTokens[j]);
		tokens.push_back(queryTokens[j]);
		leftTokens.push_back(queryTokens[j]);
	}
	splitTokens.push_back(leftTokens);
	for (int i = 0; i < wildPositions.size() - 1; ++i) {
		rightTokens.clear();
		for (int j = wildPositions[i] + 1; j <= wildPositions[i + 1] - 1; ++j) {
			allTokens.push_back(queryTokens[j]);
			tokens.push_back(queryTokens[j]);
			rightTokens.push_back(queryTokens[j]);
		}
		splitTokens.push_back(rightTokens);
	}
	vector<int> documentList = andMatch(root, allTokens);
	vector<pair<int, int>> wildcardResults;
	vector<int> rightPos, leftPos, tempPos;
	vector<int> result;
	for (int document : documentList) {
		leftPos = findExactMatchPosition(document, splitTokens[0]);
		for (int j = 1; j < splitTokens.size(); ++j) {
			rightPos = findExactMatchPosition(document, splitTokens[j]);
			tempPos.clear();
			int ir = 0, il = 0, sr = rightPos.size(), sl = leftPos.size();
			int minimumDistance = 1000000;
			for (ir = 0; ir < sr; ++ir) {
				while (il < sl - 1 && leftPos[il] + leftTokens.size() < rightPos[ir])
					il++;
				if (il && minimumDistance > (int)(rightPos[ir] - leftPos[il - 1] - leftTokens.size()))
					tempPos.push_back(rightPos[ir]);
				if (il == sl - 1)
					break;
			}
			leftPos = tempPos;
		}
		if (!leftPos.empty())
			result.push_back(document);
		/*rightPos = findExactMatchPosition(document, rightTokens);
		leftPos = findExactMatchPosition(document, leftTokens);
		int ir = 0, il = 0, sr = rightPos.size(), sl = leftPos.size();
		int minimumDistance = 1000000;
		for (il = 0; il < sl; ++il) {
			while (ir < sr - 1 && rightPos[ir] + rightTokens.size() < leftPos[il])
				ir++;
			if (ir)
				minimumDistance = min(minimumDistance, (int)(leftPos[il] - rightPos[ir - 1] - rightTokens.size()));
			if (ir == sr - 1)
				break;
		}
		if (minimumDistance != 1000000)
			wildcardResults.push_back(make_pair(document, minimumDistance));*/
	}
	//vector<int> results;
	//for (auto p : wildcardResults)
	//	results.push_back(p.first);
	return result;
}

vector<int> filetypeRanking(vector<int>& documentList, vector<string>& neededFiletypes, unordered_map<string, vector<int>>& filetypes) {
	if (neededFiletypes.empty())
		return vector<int>(documentList);
	vector<int> results;
	if (documentList.empty()) {
		for (string& filetype : neededFiletypes)
			if (!filetypes[filetype].empty()) {
				int count = 0;
				for (int doc : filetypes[filetype]) {
					results.push_back(doc);
					count++;
					if (count == 10)
						break;
				}
			}
	}
	else {
		for (int document : documentList) {
			for (string& filetype : neededFiletypes) {
				if (filetypes.find(filetype) != filetypes.end()) {
					int index = lower_bound(filetypes[filetype].begin(), filetypes[filetype].end(), document) - filetypes[filetype].begin();
					if (index < (int)filetypes[filetype].size() && filetypes[filetype][index] == document) {
						results.push_back(document);
						break;
					}
				}
			}
		}
	}
	return results;
}

vector<int> intitleRanking(vector<int>& documentList, vector<string>& intitleTokens, unordered_map<string, vector<int>>& intitleRoot) {
	if (intitleTokens.size() == 0)
		return vector<int>(documentList);
	vector<pair<int, int>> rankingList;
	int S = intitleTokens.size();
	string documentPath, firstLine, title, word;
	for (int document : documentList) {
		bool chosen = true;
		for (string& token : intitleTokens) {
			if (isDocumentInList(intitleRoot[token], document) == -1) {
				chosen = false;
				break;
			}
		}
		if (chosen) {
			documentPath = getDocumentPath(document);
			ifstream file(DOCUMENT_PREFIX + documentPath);
			int countMatchedIntitleTokens = 0;
			if (file) {
				getline(file, firstLine);
				while (firstLine == "")
					getline(file, firstLine);
				stringstream firstLineStream(firstLine);
				getline(firstLineStream, title, '.');
				stringstream titleStream(title);
				while (titleStream >> word) {
					extractWord(word);
					int index = lower_bound(intitleTokens.begin(), intitleTokens.end(), word) - intitleTokens.begin();
					if (index < S && intitleTokens[index] == word)
						++countMatchedIntitleTokens;
				}
				rankingList.push_back(make_pair(document, countMatchedIntitleTokens));
				file.close();
			}
		}
	}
	sort(rankingList.begin(), rankingList.end(), sortDecreasingSecondElement);
	vector<int> results;
	for (auto& p : rankingList)
		results.push_back(p.first);
	return results;
}