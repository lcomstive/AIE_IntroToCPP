#include <tchar.h>
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
#elif __unix__
#include <unistd.h>		// STDOUT_FILENO
#include <sys/ioctl.h>  // ioctl() and TIOCGWINSZ
#endif

void Console::Init()
{
#ifdef _WIN32
	if (s_Handle != NULL)
		return; // Already initialized
	s_Handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo = GetScreenBufferInfo();
	s_Attributes = bufferInfo.wAttributes;
#elif __unix__

#endif
}

void Console::Clear()
{
	Init();

#ifdef _WIN32
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
	Console::SetCursorPos(0, 0);
#elif __unix__
	printf("\x1B[3J\x1B[H");
#endif
}

void Console::GetScreenSize(int* width, int* height)
{
	Init();

#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo = GetScreenBufferInfo();
	*width = bufferInfo.dwSize.X;
	*height = bufferInfo.dwSize.Y;
#else
	// https://stackoverflow.com/a/23369919

	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*x = size.ws_row;
	*y = size.ws_col;
#endif
}

void Console::SetCursorPos(int x, int y)
{
	Init();

#ifdef _WIN32
	SetConsoleCursorPosition(s_Handle, COORD { (SHORT)x, (SHORT)y });
#elif __unix__
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
#elif __unix__
	printf("\033[6n"); // Writes current cursor coordinates to terminal
	scanf("\033[%d;%dR", x, y);
#endif
}

void Console::ChangeColour(ConsoleColour foreground) { ChangeColour((ConsoleColour)(s_Attributes >> 4), foreground); }
void Console::ChangeColour(ConsoleColour background, ConsoleColour foreground)
{
	Init();

#ifdef _WIN32
	SetConsoleTextAttribute(s_Handle, ((unsigned int)background << 4) | (unsigned int)foreground);
#elif __linux__

#endif
}

void Console::SetTitle(wstring title)
{
	Init();

#ifdef _WIN32
	SetConsoleTitleW(title.c_str());
#elif __linux__

#endif
}