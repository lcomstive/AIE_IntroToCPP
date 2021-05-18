#include <stdio.h>
#include <iostream>

#include "Console.hpp"

using namespace std;
using namespace TicTacToe;

#ifdef _WIN32
HANDLE Console::s_Handle = NULL;
WORD Console::s_Attributes = 0;

// Retrieve the console screen buffer for size and colours
CONSOLE_SCREEN_BUFFER_INFO Console::GetScreenBufferInfo()
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(s_Handle, &bufferInfo);
	return bufferInfo;
}
#elif __unix__ || __APPLE__
#include <unistd.h>		// STDOUT_FILENO
#include <sys/ioctl.h>  // ioctl() and TIOCGWINSZ
#endif

void Console::Init()
{
#ifdef _WIN32
	if (s_Handle != NULL)
		return; // Already initialized

	// Get console handle
	s_Handle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Grab console info (size, cursor position, font, etc.)
	s_Attributes = GetScreenBufferInfo().wAttributes;

	// Set console to UTF-8 mode
	SetConsoleOutputCP(CP_UTF8);
#endif
}

void Console::Clear()
{
	Init();

#ifdef _WIN32
	// Determine amount of cells to clear
	int width, height;
	Console::GetScreenSize(&width, &height);
	int cellCount = width * height;
	DWORD count;

	// Fill the entire buffer with empty characters
	if (!FillConsoleOutputCharacter(
		s_Handle,
		(TCHAR)' ',
		cellCount,
		COORD{ 0, 0 },
		&count
		))
		return;

	// Reset cursor
	Console::SetCursorPos(0, 0);
#elif __unix__ || __APPLE__
	printf("\033[2J\033[1;1H");
#endif
}

void Console::GetScreenSize(int* width, int* height)
{
	Init();

#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo = GetScreenBufferInfo();
	*width = bufferInfo.dwSize.X;
	*height = bufferInfo.dwSize.Y;
#elif __unix__ || __APPLE__
	// https://stackoverflow.com/a/23369919

	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*width = size.ws_row;
	*height = size.ws_col;
#endif
}

void Console::SetCursorPos(int x, int y)
{
	Init();

#ifdef _WIN32
	SetConsoleCursorPosition(s_Handle, COORD { (SHORT)x, (SHORT)y });
#elif __unix__ || __APPLE__
	printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

void Console::GetCursorPos(int* x, int* y)
{
	Init();

#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo = GetScreenBufferInfo();
	*x = bufferInfo.dwCursorPosition.X;
	*y = bufferInfo.dwCursorPosition.Y;
#elif __unix__ || __APPLE__
	printf("\033[6n"); // Writes current cursor coordinates to terminal
	scanf("\033[%d;%dR", x, y);
#endif
}

void Console::Write(std::string text, ConsoleColour foreground, ConsoleColour background)
{
#if _WIN32
	unsigned int colour = (unsigned int)foreground;
	if(background != ConsoleColour::Unchanged)
		colour = ((unsigned int)background << 4) | colour;

	SetConsoleTextAttribute(s_Handle, colour);
	cout << text;
	SetConsoleTextAttribute(s_Handle, s_Attributes);
#elif __unix__ || __APPLE__
	unsigned int fgCode = (unsigned int)foreground;
	unsigned int bgCode = (unsigned int)background + 10;

	if(foreground == ConsoleColour::Unchanged) fgCode = 0;
	if(background == ConsoleColour::Unchanged) bgCode = 0;

	printf("\033[%d;%dm%s\033[0m", bgCode, fgCode, text.c_str());
#endif
}

void Console::WriteLine(std::string text, ConsoleColour foreground, ConsoleColour background) { Console::Write(text + "\n", foreground, background); }

void Console::Write(char letter, ConsoleColour fg, ConsoleColour bg) { Console::Write(string(1, letter), fg, bg); }
void Console::WriteLine(char letter, ConsoleColour fg, ConsoleColour bg) { Console::WriteLine(string(1, letter), fg, bg); }

void Console::SetTitle(string title)
{
	Init();

#ifdef _WIN32
	SetConsoleTitle(title.c_str());
#elif __unix__ || __APPLE__
	printf("\033]0;%s", title.c_str());
#endif
}