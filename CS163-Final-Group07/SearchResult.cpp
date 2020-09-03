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

	// markedTokens only store ONE position of each matched token for better 
	// dividing of displayed words, while
	int s = tokens.size(), matchedToken = 0;
	vector<int> markedTokens(s, -1);
	int* allMatched = new int[MAXIMUM_FILE_WORDCOUNT];
	memset(allMatched, -1, MAXIMUM_FILE_WORDCOUNT);
	int wordCount = 0;
	string title;
	getline(in, title, '.');
	while (in >> word) {
		document.push_back(word);
		// Ignore non-sense words.
		if (!extractWord(word)) {
			wordCount++;
			continue;
		}
		for (int i = 0; i < s; ++i) {
			if (tokens[i] == word) {
				allMatched[wordCount] = MATCHED;
				// We only add this word document to markedTokens
				// if this token had not yet been met.
				if (markedTokens[i] == -1) {
					markedTokens[i] = wordCount;
					matchedToken++;
				}
				break;
			}
		}
		wordCount++;
	}
	cout << "\t";
	setTextColor(DARKGREEN);
	stringstream titleStream(title);
	string temporaryWord;
	int titleCount = 0, notReached = 1;
	while (titleStream >> word) {
		titleCount += word.size();
		temporaryWord = word;
		stringupper(temporaryWord);
		stringlower(word);
		if (!extractWord(word))
			cout << temporaryWord << " ";
		else {
			int index = lower_bound(tokens.begin(), tokens.end(), word) - tokens.begin();
			if (index < s && tokens[index] == word) {
				setTextColor(GREEN);
				cout << temporaryWord << " ";
				setTextColor(DARKGREEN);
			}
			else
				cout << temporaryWord << " ";
		}
		if (titleCount >= 60 && titleCount < 70) {
			cout << "...\n\t";
			notReached = 0;
			break;
		}
	}
	if (notReached)
		cout << "\n\t";
	setTextColor(GREY);
	cout << DOCUMENT_PREFIX + documentPath << "\n\n\t";
	int numOfDisplayedTokens = min(matchedToken, MAXIMUM_DISPLAYED_TOKEN);
	if (!numOfDisplayedTokens) {
		setTextColor(LIGHTBLUE);
		cout << "\n\n\n\n";
		delete[] allMatched;
		in.close();
		return;
	}
	int wordsDisplayedPerToken = MAXIMUM_DISPLAYED_WORDS / numOfDisplayedTokens;
	int lineWordCount = 0;
	for (int i = 0; i < s; ++i) {
		if (markedTokens[i] != -1) {
			int index = markedTokens[i];
			int previous = min(index, wordsDisplayedPerToken / 2);
			int after = min(wordCount - index, wordsDisplayedPerToken / 2);
			for (int j = -previous; j < after; ++j) {
				if (j == -previous && index + j != 0) {
					lineWordCount += 3;
					cout << "...";
				}
				if (allMatched[index + j] == MATCHED) {
					setTextColor(PURPLE);
					cout << document[index + j] << " ";
					setTextColor(GREY);
				}
				else 
					cout << document[index + j] << " ";
				lineWordCount += document[index + j].size() + 1;
				if (lineWordCount >= 90 && lineWordCount <= 110) {
					lineWordCount = 0;
					cout << "\n\t";
				}
				if (j == after - 1 && numOfDisplayedTokens == 1)
					cout << "...";
			}
			numOfDisplayedTokens--;
			if (!numOfDisplayedTokens)
				break;
		}
	}
	setTextColor(LIGHTBLUE);
	cout << "\n\n\n\n";
	delete[] allMatched;
	in.close();
}