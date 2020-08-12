#include "gui.h"

void setTextColor(int color) {
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;
	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
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
	cout << "\t\t\t"; cout << char(201);
	int SEARCH_BOX_WIDTH = 58;
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(187) << "\n";
	cout << "\t\t\t"; cout << char(186);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << " ";
	cout << char(186) << "\n";
	cout << "\t\t\t"; cout << char(200);
	for (int i = 0; i < SEARCH_BOX_WIDTH; ++i)
		cout << char(205);
	cout << char(188) << "\n\n\n\n\n\n\n";
	COORD searchBox;
	searchBox.X = 25;
	searchBox.Y = 11;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), searchBox);
}