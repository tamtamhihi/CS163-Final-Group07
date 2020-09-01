#include "gui.h"

void setTextColor(int color) {
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;
	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void setCursorPosition(int x, int y) {
	COORD searchResult;
	searchResult.X = x;
	searchResult.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), searchResult);
}

void setCursorToResultPosition() {
	setCursorPosition(0, 15);
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

void drawTitle() {
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

void drawSearchBox() {
	cout << "\t\t\t" << char(201);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(187) << "\n";
	cout << "\t\t\t" << char(186);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << " ";
	cout << char(186) << "\n";
	cout << "\t\t\t"; cout << char(200);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(188) << "\n\n\n\n\n\n\n";
	setCursorToSearchBox();
}

void setTextToSearchBox(string& query) {
	setCursorToSearchBox();
	cout << query;
	for (int i = 0; i < SEARCH_BOX_WIDTH - query.size(); ++i)
		cout << " ";
	setCursorToInputPosition(query.size());
}

void displayHistory(vector<string>& logs, int hightlighted) {
	cout << "\n\n";
	setTextColor(WHITE);
	int l = logs.size();
	string log;
	for (int i = 0; i < l; ++i) {
		log = logs[i];
		cout << "\t\t\t" << char(186);
		if (i == hightlighted) {
			setTextColor(GREEN);
			cout << log;
			setTextColor(WHITE);
		}
		else
			cout << log;
		for (int i = 0; i < SEARCH_BOX_WIDTH - log.size(); ++i)
			cout << " ";
		cout << char(186) << "\n";
	}
	cout << "\t\t\t"; cout << char(200);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(188);
	if (l < MAXIMUM_HISTORY_CAPACITY)
		clearLog();
	setTextColor(LIGHTBLUE);
}

void clearLog() {
	for (int j = 0; j < 8; ++j)
		cout << "                              ";
	cout << "\n";
	for (int j = 0; j < 8; ++j)
		cout << "                              ";
	cout << "\n";
	for (int j = 0; j < 8; ++j)
		cout << "                              ";
	cout << "\n";
	for (int j = 0; j < 8; ++j)
		cout << "                              ";
	cout << "\n";
}