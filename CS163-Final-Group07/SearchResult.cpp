#include "SearchResult.h"

void displayResult(int& documentNum, vector<string>& tokens) {
	ifstream in(DOCUMENT_PREFIX + "___index.txt");
	if (!in)
		return;
	// Get corresponding document path.
	string documentPath, word;
	for (int i = 0; i < documentNum; ++i)
		getline(in, documentPath);
	in.close();
	in.open(DOCUMENT_PREFIX + documentPath);
	if (!in)
		return;

	// Store content of documents in a vector to
	// easily print results.
	vector<string> document;

	// Mark the corresponding position of tokens.
	int s = tokens.size(), matchedToken = 0;
	vector<int> markedTokens(s, -1);
	int wordCount = 0;
	while (in >> word) {
		document.push_back(word);
		if (!extractWord(word)) {
			wordCount++;
			continue;
		}
		for (int i = 0; i < s; ++i) {
			if (tokens[i] == word) {
				markedTokens[i] = wordCount;
				matchedToken++;
				break;
			}
		}
		wordCount++;
	}
	int numOfDisplayedTokens = min(matchedToken, MAXIMUM_DISPLAYED_TOKEN);
	int wordsDisplayedPerToken = MAXIMUM_DISPLAYED_WORDS / numOfDisplayedTokens;
	int printedWordCount = 0;
	cout << "\t";
	for (int i = 0; i < s; ++i) {
		if (markedTokens[i] != -1) {
			int index = markedTokens[i];
			int previous = min(index, wordsDisplayedPerToken / 2);
			int after = min(wordCount - index, wordsDisplayedPerToken / 2);
			for (int j = previous; j >= 1; --j) {
				if (j == previous && index != j)
					cout << "...";
				printedWordCount++;
				cout << document[index - j] << " ";
				if (printedWordCount % 10 == 0)
					cout << "\n\t";
			}
			for (int j = 0; j < after; ++j) {
				printedWordCount++;
				cout << document[index + j] << " ";
				if (j == after - 1 && index + j != wordCount - 1)
					cout << "...";
				if (printedWordCount % 10 == 0)
					cout << "\n\t";
			}
			numOfDisplayedTokens--;
			if (!numOfDisplayedTokens)
				break;
		}
	}
	cout << "\n\n";
	in.close();
}