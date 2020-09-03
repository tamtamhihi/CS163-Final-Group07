#include "RankFunction.h"
#include "MiscProcessing.h"

bool findDoc(vector<int> documentList, int doc) {
	for (auto& document : documentList) {
		if (document == doc)
			return true;
	}
	return false;
}

// orRanking se lay tat ca cac document co trong allDocuments
// hien gio thi chi co 1 ranking do la ranking theo:
// co bao nhieu tu khac biet trong 1 document
vector<int> orRanking(vector<vector<int>>& allDocuments, int cnt) {
	if (allDocuments.size() == 0)
		return vector<int>();
	// neu chi co 1 tu thi ket qua la 1 trong nhung file do
	if (allDocuments.size() == 1) {
		int resultSize = min(SEARCH_CAPACITY, (int)allDocuments[0].size());
		vector<int> result(resultSize);
		for (int i = 0; i < resultSize; ++i) {
			result[i] = allDocuments[0][i];
		}
		return result;
	}


	int pos = 0;
	int n = allDocuments[cnt - 1].size(), m = allDocuments[cnt].size();
	// chay tu dau cho den cuoi cua mang [cnt - 1]
	for (int i = 0; i < n; ++i) {
		// neu tim duoc document trong mang [cnt - 1] thi document do duoc dua len dau
		if (i < m && findDoc(allDocuments[cnt - 1], allDocuments[cnt][i])) {
			swap(allDocuments[cnt][pos++], allDocuments[cnt][i]);
		}
		// neu khong thi dua document do vao cuoi mang
		if (!findDoc(allDocuments[cnt], allDocuments[cnt - 1][i])) {
			allDocuments[cnt].push_back(allDocuments[cnt - 1][i]);
		}
	}

	int totalDocuments = min(SEARCH_CAPACITY, (int)allDocuments[cnt].size());
	vector<int> result(totalDocuments);
	for (int i = 0; i < totalDocuments; ++i)
		result[i] = allDocuments[cnt][i];
	return result;
}


vector<int> andRanking(vector<vector<int>>& allDocuments, int cnt) {
	if (allDocuments.size() == 0) return vector<int>();

	// if there is just 1 keyword => return all documents that contain it
	if (allDocuments.size() == 1) {
		int resultSize = min(SEARCH_CAPACITY, (int)allDocuments[0].size());
		vector<int> result(resultSize);
		for (int i = 0; i < resultSize; ++i) {
			result[i] = allDocuments[0][i];
		}
		return result;
	}

	vector<int> countDocuments;
	int totalDocuments = 0;

	// loop through all the documents index that contain first keyword
	for (int i = 0; i < allDocuments[cnt].size(); ++i) {
		// neu ma khong tim thay tu do trong mang [cnt - 1] thi
		// xoa tu do o mang [cnt]
		if (!findDoc(allDocuments[cnt - 1], allDocuments[cnt][i])) {
			auto it = allDocuments[cnt].begin();
			advance(it, i);
			allDocuments[cnt].erase(it);
			--i;
			continue;
		}
		
		countDocuments.push_back(allDocuments[cnt][i]),
			totalDocuments++;
	}

	int resultSize = min(SEARCH_CAPACITY, totalDocuments);
	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i) {
		result[i] = countDocuments[i];
	}
	return result;
}

vector<int> negateRanking(vector<vector<int>>& allDocuments, int cnt) {
	if (allDocuments.size() == 0) return vector<int>();

	// if there is just 1 keyword => return all documents that DON'T contain it
	if (allDocuments.size() == 1) {
		int resultSize = min(SEARCH_CAPACITY, (int)allDocuments[0].size());
		vector<int> result;
		int i = 1;
		for (int j = 0; j < allDocuments[0].size(), result.size() < resultSize; ++j, ++i) {
			if (allDocuments[0][j] != i) {
				result.push_back(i);
			}
		}
		return result;
	}

	vector<int> countDocuments;

	// lap tu dau den cuoi cua mang truoc do
	for (int i = 0; i < allDocuments[cnt - 1].size(); ++i) {

		countDocuments.push_back(allDocuments[cnt - 1][i]);

		// neu ma mang sau cung co tu do thi xoa tu do ra khoi mang
		if (findDoc(allDocuments[cnt], allDocuments[cnt - 1][i])) {
			countDocuments.pop_back();
			auto it = allDocuments[cnt - 1].begin();
			advance(it, i);
			allDocuments[cnt - 1].erase(it);
			--i;
			continue;
		}
	}

	// cap nhap lai allDocuments
	// allDocuments[cnt] gio chi mang tat ca cac docNum co o mang [cnt - 1] ma khong co o mang [cnt]
	allDocuments[cnt] = allDocuments[cnt - 1];

	int resultSize = min(SEARCH_CAPACITY, (int)countDocuments.size());
	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i) {
		result[i] = countDocuments[i];
	}
	return result;
}

vector<int> intitleRanking(Node* root, Node* stopwordRoot, vector<string>& tokens, vector<vector<int>>& allDocumentLists, string& word, bool use, stringstream& ss) {
	int cnt = 0;
	vector<vector<int>> subDocumentLists;
	vector<string> words;

	if (use) {
		subDocumentLists.push_back(allDocumentLists.back());
		auto it = allDocumentLists.begin();
		advance(it, allDocumentLists.size() - 1);
		allDocumentLists.erase(it);
		words.push_back(word);
	}

	while (ss.good()) {
		if (!extractWord(word) || isStopword(stopwordRoot, word)) {
			continue;
		}
		vector<int> documentList = findDocumentList(root, word);
		if (!documentList.empty()) {
			tokens.push_back(word);
			words.push_back(word);
			subDocumentLists.push_back(documentList);
		}

		if (cnt > 0) {
			andRanking(subDocumentLists, cnt);
			subDocumentLists.erase(subDocumentLists.begin());
			--cnt;
		}
		++cnt;
	}

	sort(subDocumentLists[0].begin(), subDocumentLists[0].end());

	for (int i = 0; i < subDocumentLists[0].size(); ++i) {
		ifstream fin(DOCUMENT_PREFIX + to_string(subDocumentLists[0][i]) + ".txt");

		string line;
		getline(fin, line, '.');
		bool isin = true;
		for (auto& word : words) {
			if (line.find(word) == line.npos) {
				isin = false;
				break;
			}
		}

		if (!isin) {
			auto it = subDocumentLists[0].begin();
			advance(it, i);
			subDocumentLists[0].erase(it);
			--i;
		}
		fin.close();
	}
	allDocumentLists.push_back(subDocumentLists[0]);
	vector<int> searchResult = andRanking(allDocumentLists, 1);

	int resultSize = min(SEARCH_CAPACITY, (int)searchResult.size());
	vector<int> result(resultSize);
	for (int i = 0; i < resultSize; ++i) {
		result[i] = searchResult[i];
	}
	return result;
}

