/*
*
* A class to control the console, e.g. clearing and moving cursor
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
		Unchanged = 9999,

#if __unix__ || __APPLE__
		Black       = 30,
		DarkBlue    = 34,
		DarkGreen   = 32,
		DarkCyan    = 36,
		DarkRed     = 31,
		DarkMagenta = 35,
		DarkYellow  = 33,
		Grey        = 90,
		Blue        = 94,
		Green       = 92,
		Cyan        = 96,
		Red         = 91,
		Magenta     = 95,
		Yellow      = 93,
		White       = 37
#else
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
#endif
	};

	class Console
	{
	public:
		static void Clear();

		static void SetTitle(std::string title);
		static void SetCursorPos(int x, int y);
		static void GetCursorPos(int* x, int* y); // Untested
		static void GetScreenSize(int* width, int* height);

		static void Write(char letter, ConsoleColour foreground = ConsoleColour::White, ConsoleColour background = ConsoleColour::Unchanged);
		static void WriteLine(char letter, ConsoleColour foreground = ConsoleColour::White, ConsoleColour background = ConsoleColour::Unchanged);

		static void Write(std::string text, ConsoleColour foreground = ConsoleColour::White, ConsoleColour background = ConsoleColour::Unchanged);
		static void WriteLine(std::string text, ConsoleColour foreground = ConsoleColour::White, ConsoleColour background = ConsoleColour::Unchanged);

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
		/// Fills variables, gets handles.
		/// Called inherently through other functions in this class
		/// </summary>
		static void Init();
	};
}