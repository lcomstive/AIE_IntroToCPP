/*
*
* A class to control the console, e.g. clearing and moving cursor
* 
* CURRENTLY ALL UNIX CODE IS UNTESTED
* 
*/

#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
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
		static void Clear();

		static void SetTitle(std::wstring title);
		static void SetCursorPos(int x, int y);
		static void GetCursorPos(int* x, int* y);
		static void GetScreenSize(int* width, int* height);

		static void ChangeColour(ConsoleColour foreground);
		static void ChangeColour(ConsoleColour background, ConsoleColour foreground);

	private:
#if _WIN32
		// WINDOWS API //
		static WORD s_Attributes; // Stores values such as current console colour
		static HANDLE s_Handle;	  // Handle to the console process

		/// <summary>
		/// Retrieves information on the current console buffer, which includes size and colours.
		/// </summary>
		/// <returns></returns>
		static CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo();
#endif

		/// <summary>
		// Fills variables, gets handles.
		// Called inherintly through other functions in this class
		/// </summary>
		static void Init();
	};
}