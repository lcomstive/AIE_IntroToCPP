/*

Copyright 2021 Lewis Comstive

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

-----------------------------------------------------------------

A class to control the console, e.g. clearing and moving cursor
 
CURRENTLY ALL UNIX CODE IS UNTESTED
	
*/

#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // Windows.h is bloated, this reduces it a bit
#include <Windows.h>
#endif
#include <string>

namespace TicTacToe
{
	enum class ConsoleColour : unsigned int
	{
		Black		= 0,
		DarkBlue	= 1,
		DarkGreen	= 2,
		DarkCyan	= 3,
		DarkRed		= 4,
		DarkMagenta	= 5,
		DarkYellow	= 6,
		Grey		= 7,
		Blue		= 9,
		Green		= 10,
		Cyan		= 11,
		Red			= 12,
		Magenta		= 13,
		Yellow		= 14,
		White		= 15
	};

	class Console
	{
	public:
		static void Init();

		static void Clear();
		static void SetTitle(std::wstring title);
		static void SetCursorPos(int x, int y);
		static void GetCursorPos(int* x, int* y);
		static void GetScreenSize(int* width, int* height);

		static void ChangeColour(ConsoleColour foreground);
		static void ChangeColour(ConsoleColour background, ConsoleColour foreground);

	private:
#if _WIN32
		static WORD s_Attributes;
		static HANDLE s_Handle;

		static CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo();
#endif
	};
}