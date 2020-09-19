#include "gui.h"

// -------- CURSOR AND CONSOLE MANIPULATION --------

// Change console text color.
void setTextColor(int color) {
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;
	SetConsoleTextAttribute(hConsoleOutput, color);
}

// Change console cursor to corresponding coordinates.
void setCursorPosition(int x, int y) {
	COORD searchResult;
	searchResult.X = x;
	searchResult.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), searchResult);
}
void setCursorToResultPosition() {
	setCursorPosition(RESULT_X, RESULT_Y);
}
void setCursorToSearchBox() {
	setCursorPosition(SEARCH_BOX_X, SEARCH_BOX_Y);
}
void setCursorToLog() {
	setCursorPosition(SEARCH_BOX_X + 1, SEARCH_BOX_Y + 2);
}
void setCursorToInputPosition(int queryLength) {
	setCursorPosition(SEARCH_BOX_X + queryLength, SEARCH_BOX_Y);
}

// -------- DRAW UI --------

// Search engine title.
void drawTitle() {
	cout << "\n";
	cout << "\t\t\t\t"; cout << "  "; setTextColor(GREEN); cout << "  #### "; setTextColor(RED); cout << " ######"; setTextColor(YELLOW); cout << " ##   ##"; setTextColor(WHITE); cout << " ######"; setTextColor(LIGHTBLUE); cout << " ##    ##" << endl;
	cout << "\t\t\t\t"; cout << "  "; setTextColor(GREEN); cout << " ##    "; setTextColor(RED); cout << " ##    "; setTextColor(YELLOW); cout << " ##   ##"; setTextColor(WHITE); cout << " ##    "; setTextColor(LIGHTBLUE); cout << " ###   ##" << endl;
	cout << "\t\t\t\t"; cout << "  "; setTextColor(GREEN); cout << "  #### "; setTextColor(RED); cout << " ####  "; setTextColor(YELLOW); cout << " ##   ##"; setTextColor(WHITE); cout << " ####  "; setTextColor(LIGHTBLUE); cout << " ## #  ##" << endl;
	cout << "\t\t\t\t"; cout << "  "; setTextColor(GREEN); cout << "     ##"; setTextColor(RED); cout << " ##    "; setTextColor(YELLOW); cout << "  ## ## "; setTextColor(WHITE); cout << " ##    "; setTextColor(LIGHTBLUE); cout << " ##  # ##" << endl;
	cout << "\t\t\t\t"; cout << "  "; setTextColor(GREEN); cout << "  #### "; setTextColor(RED); cout << " ######"; setTextColor(YELLOW); cout << "   ###  "; setTextColor(WHITE); cout << " ######"; setTextColor(LIGHTBLUE); cout << " ##   ###" << endl;
	setTextColor(RED);
	cout << "\t\t\t\t"; cout << "  #### "; setTextColor(WHITE); cout << " ######"; setTextColor(PURPLE); cout << "  #### "; setTextColor(LIGHTBLUE); cout << " ##### "; setTextColor(YELLOW); cout << "  #### "; setTextColor(RED); cout << " ##  ##" << endl;
	cout << "\t\t\t\t"; cout << " ##    "; setTextColor(WHITE); cout << " ##    "; setTextColor(PURPLE); cout << " ##  ##";  setTextColor(LIGHTBLUE); cout << " ##  ##"; setTextColor(YELLOW); cout << " ##  ##"; setTextColor(RED); cout << " ##  ##" << endl;
	cout << "\t\t\t\t"; cout << "  #### "; setTextColor(WHITE); cout << " ####  "; setTextColor(PURPLE); cout << " ######"; setTextColor(LIGHTBLUE); cout << " ##### "; setTextColor(YELLOW); cout << " ##    "; setTextColor(RED); cout << " ######" << endl;
	cout << "\t\t\t\t"; cout << "     ##"; setTextColor(WHITE); cout << " ##    "; setTextColor(PURPLE); cout << " ##  ##"; setTextColor(LIGHTBLUE); cout << " ##  ##"; setTextColor(YELLOW); cout << " ##  ##"; setTextColor(RED); cout << " ##  ##" << endl;
	cout << "\t\t\t\t"; cout << "  #### "; setTextColor(WHITE); cout << " ######"; setTextColor(PURPLE); cout << " ##  ##"; setTextColor(LIGHTBLUE); cout << " ##  ##"; setTextColor(YELLOW); cout << "  #### "; setTextColor(RED); cout << " ##  ##" << endl;
	setTextColor(LIGHTBLUE);
}

// Engine menu.
void drawMenu(int highlightedChoice) {
	setTextColor(LIGHTBLUE);
	vector<string> menuOptions = { "SEARCH", "GUIDES", "ADD DOCS", "EXIT"};
	cout << "\n\n";
	for (int i = 0; i < menuOptions.size(); ++i) {
		cout << "\t\t\t\t\t     " << char(201);
		for (int j = 0; j < MENU_BOX_WIDTH; ++j)
			cout << char(205);
		cout << char(187) << "\n\t\t\t\t\t     " << char(186);
		int offset = (MENU_BOX_WIDTH - menuOptions[i].size()) / 2;
		for (int j = 0; j < offset; ++j)
			cout << " ";
		if (i != highlightedChoice) {
			setTextColor(GREY);
			cout << menuOptions[i];
			setTextColor(LIGHTBLUE);
		}
		else
			cout << menuOptions[i];
		for (int j = 0; j < offset; ++j)
			cout << " ";
		cout << char(186) <<  "\n\t\t\t\t\t     " << char(200);
		for (int j = 0; j < MENU_BOX_WIDTH; ++j)
			cout << char(205);
		cout << char(188) << "\n\n";
	}
	setTextColor(LIGHTBLUE);
}

// Search box.
void drawSearchBox() {
	cout << "\t\t\t" << char(201);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(187) << "\n\t\t\t" << char(186);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << " ";
	cout << char(186) << "\n\t\t\t" << char(200);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(188);
	setCursorToSearchBox();
}

// Input box when adding new document.
void drawInputBox(int height) {
	cout << char(201);
	for (int i = 0; i < INPUT_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(187) << "\n";
	for (int h = 0; h < height; ++h) {
		cout << char(186);
		for (int i = 0; i < INPUT_BOX_WIDTH; ++i)
			cout << " ";
		cout << char(186) << "\n";
	}
	cout << char(200);
	for (int i = 0; i < INPUT_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(188) << "\n\n\n\n\n\n\n";
	setCursorPosition(INPUT_BOX_X, INPUT_BOX_Y);
}


// -------- UI INTERACTION AND MANIPULATION --------

// Set certain text to search box and set cursor to end position.
void setTextToSearchBox(string& query) {
	setCursorToSearchBox();
	cout << query;
	for (int i = 0; i < SEARCH_BOX_WIDTH - query.size(); ++i)
		cout << " ";
	setCursorToInputPosition(query.size());
}

// Display history with default no highlighted option.
void displayHistory(vector<string>& logs, int hightlighted) {
	cout << "\n\n";
	setTextColor(WHITE);
	int l = logs.size();
	string log;
	setTextColor(LIGHTBLUE);
	for (int i = 0; i < l; ++i) {
		log = logs[i];
		cout << "\t\t\t" << char(186);
		setTextColor(WHITE);
		if (i == hightlighted)
			setTextColor(GREEN);
		cout << log;
		for (int i = 0; i < SEARCH_BOX_WIDTH - log.size(); ++i)
			cout << " ";
		setTextColor(LIGHTBLUE);
		cout << char(186) << "\n";
	}
	cout << "\t\t\t"; cout << char(200);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(188) << "\n";
	clearLog();
}

// Clear logs.
void clearLog() {
	for (int j = 0; j < 7; ++j)
		cout << "                              ";
	cout << endl;
	for (int j = 0; j < 7; ++j)
		cout << "                              ";
	cout << endl;
	for (int j = 0; j < 7; ++j)
		cout << "                              ";
	cout << endl;
	for (int j = 0; j < 7; ++j)
		cout << "                              ";
	cout << endl;
	for (int j = 0; j < 7; ++j)
		cout << "                              ";
	cout << endl;
}