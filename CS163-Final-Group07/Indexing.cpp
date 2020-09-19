#include "Indexing.h"

// This function open a filepath to get all filepaths of added documents.
// It extracts a filename from each filepath, copy from the original
// location to ProjectData and add a new document to the index trie as well 
// as filetype unordered_map so that the current running program can search 
// properly, and rewrite the index.txt and filetype.txt file.
void addNewDocumentsToIndex(
	Node*& root,
	unordered_map<string, vector<int>>& intitleRoot,
	Node*& stopwordRoot,
	unordered_map<string, vector<int>>& filetypes,
	string& filepath) {

	cout << "\n";
	ifstream filepathStream(filepath);
	ofstream documentPath(DOCUMENTPATH_FILE, ios::app);
	if (!filepathStream)
		return;
	string addedFilePath, filename, filetype;
	int totalDocument = getTotalDocumentNum();
	int addedDocument = 0;
	while (getline(filepathStream, addedFilePath)) {
		// Add to ____index.txt.
		documentPath << addedFilePath << "\n";

		// Get current filename and copy file from original location to ProjectData folder.
		filename = getFilenameFromPath(addedFilePath);
		ifstream in(filepath, ios::binary);
		if (!in)
			continue;
		ofstream out(DOCUMENT_PREFIX + filename, ios::binary);
		copyFromInToOut(in, out);
		in.close();
		out.close();

		// Index to root.
		inspectDocument(root, intitleRoot, filename, totalDocument, stopwordRoot);
		addedDocument++;

		// Change filetypes.
		filetype = getFiletypeFromFilepath(filename);
		filetypes[filetype].push_back(totalDocument);
	}
	documentPath.close();
	
	// Change numdoc.txt.
	changeTotalDocumentNum(totalDocument + addedDocument);

	// Rewrite filetype.txt.
	printFiletypeUnorderedMap(filetypes);
	cout << "\tAdding to filetype file...\n";

	// Rewrite index.txt.
	ofstream indexFile(INDEX_FILE);
	string word = "";
	printIndexToFile(indexFile, root, word);
	indexFile.close();
}


// This function traverse a new word and add this document number to the final node.
void addWordWithDocumentNum(string& word, Node*& root, int& documentNum) {
	Node* addedWord = addWordReturnFinal(word, root);
	// currentNode now points at the final node.
	addedWord->isWord = 1;
	if (!addedWord->documentList.empty() && addedWord->documentList.back() == documentNum)
		return;
	addedWord->documentList.push_back(documentNum);
}


// This function traverse the word and add to trie with the given document list.
void addWordWithDocumentList(string& word, Node*& root, vector<int>& documentList) {
	Node* addedWord = addWordReturnFinal(word, root);
	addedWord->isWord = 1;
	addedWord->documentList = documentList;
}

// This function changes the file numdoc.txt.
void changeTotalDocumentNum(int newDocumentNum) {
	ofstream out(DOCNUM_FILE);
	out << newDocumentNum << "\n";
	out.close();
}

// This function indexes all documents into a trie, save to index.txt
// and extract filetypes to save to filetype.txt.
void indexing() {
	// Create a root node for main trie.
	Node* root = newNode();
	unordered_map<string, vector<int>> filetypes, intitleRoot;
	// Create a root node for stopwords.
	Node* stopwordRoot = newNode();
	loadStopwords(stopwordRoot);
	ifstream allDocuments(DOCUMENT_PREFIX + "___index.txt");
	if (allDocuments) {
		string documentPath;
		int documentCount = 0;
		while (getline(allDocuments, documentPath)) {
			string filetype = getFiletypeFromFilepath(documentPath);
			documentCount++;
			if (documentCount % 200 == 0)
				cout << "Crawled " << documentCount << "docs...\n";
			inspectDocument(root, intitleRoot, documentPath, documentCount, stopwordRoot);
			filetypes[filetype].push_back(documentCount);
		}
		cout << "Finished crawling " << documentCount << " documents...\n";

		// Add all words from trie to index file.
		cout << "Adding index to file...\n\n";
		string word = "";
		ofstream indexFile(INDEX_FILE, ios::app);
		printIndexToFile(indexFile, root, word);
		indexFile.close();
		allDocuments.close();
		printFiletypeUnorderedMap(filetypes);
		printIntitleUnorderedMap(intitleRoot);
	}
	// Delete trie from memory.
	deleteTrie(root);
	deleteTrie(stopwordRoot);
}

// This function indexes all documents into an unordered_map, save to index.txt
// as state 0 (unsorted) in props.txt. This also print filetype to filetype.txt.
void indexingUnorderedMap() {
	unordered_map<string, vector<int>> root, intitleRoot, filetypes;
	Node* stopwordRoot = newNode();
	loadStopwords(stopwordRoot);
	ifstream allDocuments(DOCUMENT_PREFIX + "___index.txt");
	if (allDocuments) {
		string documentPath;
		int documentCount = 0;
		while (getline(allDocuments, documentPath)) {
			string filetype = getFiletypeFromFilepath(documentPath);
			documentCount++;
			if (documentCount % 200 == 0)
				cout << "Crawled " << documentCount << "docs...\n";
			inspectDocumentUnorderedMap(root, intitleRoot, documentPath, documentCount, stopwordRoot);
			filetypes[filetype].push_back(documentCount);
		}
		cout << "Finished crawling " << documentCount << " documents...\n";
		changeTotalDocumentNum(documentCount);
		// Add all words from trie to index file.
		cout << "Adding index to file...\n\n";
		string word = "";
		ofstream indexFile(INDEX_FILE, ios::app);
		printIndexToFileUnorderedMap(indexFile, root);
		indexFile.close();
		allDocuments.close();
		printFiletypeUnorderedMap(filetypes);
		printIntitleUnorderedMap(intitleRoot);
	}
	deleteTrie(stopwordRoot);
}

// This function opens document and read in all words to add to index trie.
void inspectDocument(
	Node*& root,  
	unordered_map<string, vector<int>>& intitleRoot,
	string& documentPath, 
	int& documentNum, Node*& stopwordRoot) {

	// Open corresponding file.
	ifstream in(DOCUMENT_PREFIX + documentPath);
	if (!in)
		return;
	string word, firstLine, title;
	getline(in, firstLine);
	while (firstLine == "")
		getline(in, firstLine);
	stringstream firstLineStream(firstLine);
	getline(firstLineStream, title, '.');
	stringstream titleStream(title);

	while (titleStream >> word) {
		if (!extractWord(word))
			continue;
		addWordWithDocumentNum(word, root, documentNum);
		if (!intitleRoot[word].empty() && intitleRoot[word].back() == documentNum)
			continue;
		intitleRoot[word].push_back(documentNum);
	}

	// Read in word by word until EOF.
	while (firstLineStream >> word || in >> word) {
		if (word == "")
			break;
		// Extract the word and determine if the word should be added.
		if (!extractWord(word))
			continue;
		addWordWithDocumentNum(word, root, documentNum);
	}
	in.close();
}

void removeNullFiles() {
	ifstream filename(DOCUMENTPATH_FILE);
	if (filename) {
		vector<string> documentPath;
		string path, word;
		while (getline(filename, path)) {
			ifstream file(DOCUMENT_PREFIX + path);
			if (file) {
				file >> word;
				if (extractWord(word))
					documentPath.push_back(path);
				file.close();
			}
		}
		filename.close();
		ofstream filenames(DOCUMENTPATH_FILE);
		for (string& p : documentPath) {
			filenames << p << "\n";
		}
		filenames.close();
	}
}

// This function opens document and read in all words to add to index unordered_map.
void inspectDocumentUnorderedMap(
	unordered_map<string, vector<int>>& root, 
	unordered_map<string, vector<int>>& intitleRoot, 
	string& documentPath,
	int& documentNum, Node*& stopwordRoot
) {

	// Open corresponding file.
	ifstream in(DOCUMENT_PREFIX + documentPath);
	if (!in)
		return;
	string word, firstLine, title;
	getline(in, firstLine);
	while (firstLine.empty()) {
		if (documentNum > 3600) {
			cout << documentNum << "not";
		}
		getline(in, firstLine);
	}
	stringstream firstLineStream(firstLine);
	getline(firstLineStream, title, '.');
	stringstream titleStream(title);

	while (titleStream >> word) {
		if (!extractWord(word))
			continue;
		if (!intitleRoot[word].empty() && intitleRoot[word].back() == documentNum)
			continue;
		intitleRoot[word].push_back(documentNum);
		root[word].push_back(documentNum);
	}

	// Read in word by word until EOF.
	while (firstLineStream >> word || in >> word) {
		if (word == "")
			break;
		// Extract the word and determine if the word should be added.
		if (!extractWord(word))
			continue;
		if (!root[word].empty() && root[word].back() == documentNum)
			continue;
		root[word].push_back(documentNum);
	}
	in.close();
}


// This function print the whole indexed Trie to a file in PreprocessedData/index.txt.
// To make this process more efficient, we use a string to store the word so far.
void printIndexToFile(ofstream& indexFile, Node*& root, string& word) {
	// If the current node is the final node of a word,
	// print out that word together with document list.
	if (root->isWord != -1) {
		//cout << "Writing \"" << word << "\"...\n";
		indexFile << word;
		for (int documentNum : root->documentList)
			indexFile << " " << documentNum;
		indexFile << "\n";
		// We do not return here because there might be more words behind.
		// For example, "and" and "andy".
	}
	// Go through all children and continue adding new characters to the word.
	map<int, Node*>::iterator iterator;
	for (iterator = root->child.begin(); iterator != root->child.end(); ++iterator) {
		word += char(iterator->first);
		printIndexToFile(indexFile, iterator->second, word);
		word.pop_back();
	}
}

// Print the index unordered_map to index.txt as state 0 (unsorted) in props.txt.
void printIndexToFileUnorderedMap(ofstream& indexFile, unordered_map<string, vector<int>>& root) {
	for (auto& word : root) {
		indexFile << word.first;
		for (int doc : word.second)
			indexFile << " " << doc;
		indexFile << "\n";
	}
	// Mark that index file is not sorted.
	ofstream propsFile(PROPS_FILE);
	if (propsFile) {
		propsFile << "0\n";
		propsFile.close();
	}
}

// Print unordered_map to filetype.txt.
void printFiletypeUnorderedMap(unordered_map<string, vector<int>>& filetypes) {
	ofstream filetypeFile(FILETYPE_FILE);
	for (auto& filetype : filetypes) {
		filetypeFile << filetype.first;
		for (int& documentNum : filetype.second)
			filetypeFile << " " << documentNum;
		filetypeFile << "\n";
	}
	filetypeFile.close();
}

// Print unordered_map to intitle.txt
void printIntitleUnorderedMap(unordered_map<string, vector<int>>& intitleRoot) {
	ofstream intitleFile(INTITLE_FILE);
	for (auto& intitle : intitleRoot) {
		intitleFile << intitle.first;
		for (int& documentNum : intitle.second)
			intitleFile << " " << documentNum;
		intitleFile << "\n";
	}
	intitleFile.close();
}

// This function sort the synonyms lists.
void preprocessSynonyms() {
	ifstream synonymFile(SYNONYM_FILE);
	if (synonymFile) {
		string line, word;
		vector<vector<string>> synonyms;
		int lineCount = 0;
		while (getline(synonymFile, line)) {
			stringstream lineStream(line);
			synonyms.push_back(vector<string>());
			while (lineStream >> word) {
				synonyms[lineCount].push_back(word);
			}
			sort(synonyms[lineCount].begin(), synonyms[lineCount].end());
			lineCount++;
		}
		synonymFile.close();
		ofstream outFile(SYNONYM_FILE);
		for (int i = 0; i < lineCount; ++i) {
			for (string& word : synonyms[i])
				outFile << word << " ";
			outFile << "\n";
		}
		outFile.close();
	}
}

// Extract the filetype from a filename/filepath.
string getFiletypeFromFilepath(string& documentPath) {
	string filetype = "";
	for (int j = documentPath.size() - 1; j >= 0; --j) {
		if (documentPath[j] == '.')
			break;
		filetype = documentPath[j] + filetype;
	}
	return filetype;
}

// This function retrieve data from "index.txt" into a Trie to be ready for searching.
void retrieve(Node*& root) {
	// Open index file.
	ifstream in(INDEX_FILE);
	if (!in)
		return;

	string line, token;
	vector<int> documents;
	int documentNum = 0;

	// Read in line by line until EOF.
	while (getline(in, line)) {
		// Split the line into tokens
		// First token will hold the keyword
		// others will hold the document number
		stringstream tokenLine(line);
		tokenLine >> token;
		documents.clear();
		while (tokenLine >> documentNum)
			documents.push_back(documentNum);
		// Add word to root.
		addWordWithDocumentList(token, root, documents);
	}
	cout << "\t\t\t\tFinish retrieving index...\n\n";
	in.close();
}

// Retrieve index.txt into an unordered_map.
void retrieveUnorderedMap(unordered_map<string, vector<int>>& root) {
	// Open index file.
	ifstream in(INDEX_FILE);
	if (!in)
		return;
	string line, token;
	vector<int> documents;
	int documentNum = 0;

	// Read in line by line until EOF.
	while (getline(in, line)) {
		// Split the line into tokens
		// First token will hold the keyword
		// others will hold the document number
		stringstream tokenLine(line);
		tokenLine >> token;
		documents.clear();
		while (tokenLine >> documentNum)
			documents.push_back(documentNum);
		// Add word to root.
		root[token] = documents;
	}
	in.close();
}

// Load filetype.txt into an unordered_map.
void loadFiletypes(unordered_map<string, vector<int>>& filetypes) {
	ifstream filetypesFile(FILETYPE_FILE);
	if (filetypesFile) {
		string filetype, line;
		int documentNum;
		while (filetypesFile >> filetype) {
			getline(filetypesFile, line);
			stringstream lineStream(line);
			while (lineStream >> documentNum)
				filetypes[filetype].push_back(documentNum);
		}
		filetypesFile.close();
	}
}

// Get current number of indexed documents from numdoc.txt.
int getTotalDocumentNum() {
	ifstream in(DOCNUM_FILE);
	if (in) {
		int docNum;
		in >> docNum;
		in.close();
		return docNum;
	}
	return 0;
}

// Add new filename in ProjectData to ___index.txt.
void addNewDocumentName(string filename, int currentTotalDocumentNum) {
	ofstream documentPath(DOCUMENTPATH_FILE, ios::app);
	documentPath << filename << "\n";
	changeTotalDocumentNum(currentTotalDocumentNum + 1);
	documentPath.close();
}

// This function copies a whole file from a binary ifstream
// to a binary ofstream.
void copyFromInToOut(ifstream& in, ofstream& out) {
	// Read 100 characters a time until cannot read.
	while (true) {
		unsigned char* s = new unsigned char[100];
		in.read((char*)s, 100);
		int count = in.gcount();
		// If cannot read any more letter, exit the loop.
		if (!count) {
			delete[] s;
			break;
		}
		out.write((char*)s, count);
		delete[] s;
	}
}

// Extract the filename from a filepath.
string getFilenameFromPath(string& filepath) {
	string filename = "";
	for (int i = filepath.size() - 1; i >= 0; --i) {
		if (filepath[i] == '/' || filepath[i] == '\\')
			break;
		filename = filepath[i] + filename;
	}
	return filename;
}

