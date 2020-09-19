#include "SearchResult.h"

void displayResult(int& documentNum, vector<string>& tokens, vector<string>& intitleTokens) {

	// Get corresponding document path.
	string documentPath, word;
	documentPath = getDocumentPath(documentNum);
	ifstream in(DOCUMENT_PREFIX + documentPath);
	if (!in)
		return;

	vector<string> document;

	// markedTokens only store ONE position of each matched token for better 
	// dividing of displayed words.
	int s = tokens.size(), matchedToken = 0;
	vector<int> markedTokens(s, -1);

	// allMatched store ALL positions that match one of the tokens.
	int* allMatched = new int[MAXIMUM_FILE_WORDCOUNT];
	memset(allMatched, -1, MAXIMUM_FILE_WORDCOUNT);

	int wordCount = 0;

	// Get the title (the first line OR the first sentence ending with '.').
	string firstLine, title;
	getline(in, firstLine);
	while (firstLine == "")
		getline(in, firstLine);
	stringstream firstLineStream(firstLine);
	getline(firstLineStream, title, '.');

	// The remaining word (if any) in the first line will be considered the content.
	// Mark the positions of matched tokens
	while (firstLineStream >> word || in >> word) {
		document.push_back(word);

		// Ignore non-sense words.
		if (!extractWord(word)) {
			wordCount++;
			continue;
		}

		// Mark the matched tokens.
		int index = isTokenInList(word, tokens);
		if (index != -1) {
			allMatched[wordCount] = MATCHED;
			// We only add this word document to markedTokens
			// if this token had not yet been met.
			if (markedTokens[index] == -1) {
				markedTokens[index] = wordCount;
				matchedToken++;
			}
		}
		wordCount++;
	}
	in.close();

	// Printing the title.
	cout << "\t";
	setTextColor(DARKGREEN);
	stringstream titleStream(title);
	string temporaryWord;

	// Have tokens to match in title.
	if (!intitleTokens.empty()) {
		vector<string> titleDocument;
		vector<int> allMatchedTitle;
		vector<pair<int, int>> matchedPair;
		int titleWordCount = 0;

		// We read every word into a vector
		while (titleStream >> word) {
			temporaryWord = word;
			stringupper(temporaryWord);
			titleDocument.push_back(temporaryWord);
			extractWord(word);
			int temp = isTokenInList(word, intitleTokens);
			if (temp != -1) {
				matchedPair.push_back(make_pair(temp, titleWordCount));
				allMatchedTitle.push_back(MATCHED);
			}
			else
				allMatchedTitle.push_back(-1);
			++titleWordCount;
		}
		sort(matchedPair.begin(), matchedPair.end(), sortIncreasingSecondElement);
		int l = 0, r, ls = -1, rs = -1, maxDist = 0, dist = 0;
		for (r = 0; r < (int)matchedPair.size(); ++r) {
			dist = matchedPair[r].second - matchedPair[l].second + 1;
			while (l < r && dist > MAXIMUM_DISPLAYED_TITLE_WORDCOUNT) {
				++l;
				dist = matchedPair[r].second - matchedPair[l].second + 1;
			}
			if (dist > maxDist) {
				maxDist = dist;
				rs = r, ls = l;
			}
		}
		if (ls)
			cout << "...";
		int extraTitleCount = MAXIMUM_DISPLAYED_TITLE_WORDCOUNT - (int)matchedPair[rs].second + (int)matchedPair[ls].second - 1;
		ls = max(0, matchedPair[ls].second - extraTitleCount / 2);
		rs = min(titleWordCount - 1, matchedPair[rs].second + extraTitleCount / 2);
		for (int i = ls; i <= rs; ++i) {
			if (i == ls && i)
				cout << "...";
			if (allMatchedTitle[i] == MATCHED) {
				setTextColor(GREEN);
				cout << titleDocument[i];
				setTextColor(DARKGREEN);
			}
			else
				cout << titleDocument[i];
			cout << " ";
			cout << " ";
			if (i == rs)
				cout << "...";
		}
		cout << "\n\t";
	}
	else {
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
			if (titleCount >= 60) {
				cout << "...\n\t";
				notReached = 0;
				break;
			}
		}
		if (notReached)
			cout << "\n\t";
	}

	// Printing the document path.
	setTextColor(GREY);
	cout << DOCUMENT_PREFIX + documentPath << "\n\n\t";

	// Printing the content.
	int numOfDisplayedTokens = min(matchedToken, MAXIMUM_DISPLAYED_TOKEN);
	int lineWordCount = 0;
	if (!numOfDisplayedTokens) {
		for (int i = 0; i < MAXIMUM_DISPLAYED_WORDS; ++i) {
			cout << document[i] << " ";
			lineWordCount += document[i].size() + 1;
			if (lineWordCount >= 90) {
				lineWordCount = 0;
				cout << "\n\t";
			}
		}
		cout << "...";
	}
	else {
		int wordsDisplayedPerToken = MAXIMUM_DISPLAYED_WORDS / numOfDisplayedTokens;
		// Each token in list will account for a batch of words.
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
					// Matched words that have been printed
					// will have corresponding matchedTokens turned to -1.
					// This means that 1 token can only printed once.
					if (allMatched[index + j] == MATCHED) {
						temporaryWord = document[index + j];
						extractWord(temporaryWord);
						int tempInd = isTokenInList(temporaryWord, tokens);
						if (tempInd != -1)
							markedTokens[tempInd] = -1;
						setTextColor(PURPLE);
						cout << document[index + j] << " ";
						setTextColor(GREY);
					}
					else
						cout << document[index + j] << " ";

					lineWordCount += document[index + j].size() + 1;

					// Break new line.
					if (lineWordCount >= 90) {
						lineWordCount = 0;
						cout << "\n\t";
					}
				}
				numOfDisplayedTokens--;
				if (!numOfDisplayedTokens)
					break;
			}
		}
		cout << "...";
	}

	setTextColor(LIGHTBLUE);
	cout << "\n\n\n\n";
	delete[] allMatched;
}