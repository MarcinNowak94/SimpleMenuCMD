#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>


#ifndef CLS_H
#define CLS_H

void cls(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer. 
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.
	if (!FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Get the current text attribute.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	// Set the buffer's attributes accordingly.
	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition(hConsole, coordScreen);
};

#endif //!CLS_H

#ifndef SIMPLEMENUDISPLAY_H
#define SIMPLEMENUDISPLAY_H

template<class T, class T2, class T3, class T4 = char*>
int simplemenudisplay(std::vector<bool> menu, const T menuoptions[], T2 amountofoptions, T3 cursorposition, T4 header = "")
{
	char uicursor[2]{ ' ', '*' };
	char input{};
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	do
	{

		cls(hStdout);
		if (header != "") { std::cout << header << "\n"; };
		std::cout << "------------------------------------------------------------\n";
		for (T2 i = 0; i <= amountofoptions; i++)
		{
			if (i == amountofoptions)
			{
				std::cout << "------------------------------------------------------------\n"
					<< "[" << uicursor[menu.at(i)] << "] Back\\Exit\n";
			}
			else
			{
				std::cout << "[" << uicursor[menu.at(i)] << "] " << menuoptions[i] << "\n";
			}

		}
		input = _getch();
		if (input == '0' || input == '0xE0') { input = _getch(); };		//arrow keys are precieded either by 0 or 0xE0 in input
		switch (input)
		{
		case 72: {if (cursorposition != 0)			//up arrow
		{
			menu[cursorposition - 1] = menu[cursorposition];
			menu[cursorposition] = 0;
			cursorposition -= 1;
		}; }; break;
		case 80: {if (cursorposition != amountofoptions)			//down arrow
		{
			menu[cursorposition + 1] = menu[cursorposition];
			menu[cursorposition] = 0;
			cursorposition += 1;

		}; }; break;
		case '\r':									//enter
		{
			return cursorposition;
		}; break;
		default: break;
		};
	} while (input != 27 || input != amountofoptions);
	return 27;
};

#endif // !SIMPLEMENUDISPLAY_H


#ifndef SIMPLEMENU_H
#define SIMPLEMENU_H

template<class T>
int simplemenu(T & optionstodisplay)
{
	//TODO: calculate amount of options
	std::vector<bool> menu(1);	//temporary vector remembering position of cursor
	menu.at(0) = 1;					//initializing cursor position at 1st element
	for (auto i = 0; i <= (sizeof(optionstodisplay) / sizeof(*optionstodisplay)); i++, menu.push_back(0));		//extra element for "back\exit"
	int cursorpos = 0;
	cursorpos = simplemenudisplay(menu, optionstodisplay, (sizeof(optionstodisplay) / sizeof(*optionstodisplay)), cursorpos);
	return cursorpos;
}

template<class T, class T2>
int simplemenu(T & optionstodisplay, T2 & header)
{
	//TODO: calculate amount of options
	std::vector<bool> menu(1);	//temporary vector remembering position of cursor
	menu.at(0) = 1;					//initializing cursor position at 1st element
	for (auto i = 0; i < (sizeof(optionstodisplay) / sizeof(*optionstodisplay)); i++)
	{
		menu.push_back(0);
	};
	int cursorpos = 0;
	cursorpos = simplemenudisplay(menu, optionstodisplay, (sizeof(optionstodisplay) / sizeof(*optionstodisplay)), cursorpos, header);
	return cursorpos;
}
#endif // !SIMPLEMENU_H