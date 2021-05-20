#include <string>
#include <iostream>
#include "Game.hpp"
#include "Console.hpp"

using namespace std;
using namespace TicTacToe;

// "Inspired" by https://gist.github.com/plasticbox/3708a6cdfbece8cd224487f9ca9794cd
string GetArgument(const int argc, const char** argv, const string& option)
{
	for (int i = 0; i < argc; i++)
	{
		string arg = argv[i];
		if (arg.find(option) == 0)
			return arg.substr(option.size() + 1);
	}

	return "";
}

int GetArgumentInt(const int argc, const char** argv, const string& option, int defaultValue = 0)
{
	const string raw = GetArgument(argc, argv, option);
	return raw.empty() ? defaultValue : stoi(raw); // stoi is a C++11 feature
}

GameArgs ParseArguments(const int argc, const char** argv)
{
	GameArgs args;
	args.BoardSize   = GetArgumentInt(argc, argv, "--board-size", 3);
	args.PlayerCount = GetArgumentInt(argc, argv, "--player-count", 2);
	args.AiPlayerCount = GetArgumentInt(argc, argv, "--ai-players");
	return args;
}

int main(const int argc, const char** argv)
{
	GameArgs args = ParseArguments(argc, argv);
	if ((args.PlayerCount + args.AiPlayerCount) < 2)
		args.AiPlayerCount = 2 - args.PlayerCount; // Make sure there are at least 2 players, even if both are AI

	Game game(args);
	const unsigned int boardSize = game.GetBoardSize();

	string title = "Tic Tac Toe Game - Lewis Comstive";
	title += " (" + to_string(boardSize) + "x" + to_string(boardSize) + ")";
	title += " (Players: " + to_string(game.GetPlayerCount()) + ")";
	title += "(AI: " + to_string(game.GetAiPlayerCount()) + ")";

	Console::SetTitle(title);

	string previousMove;
	while (game.IsRunning())
	{
		game.Draw();

		if (args.PlayerCount == 0)
			continue; // Don't get user input, there are no players here

		string line;
		getline(cin, line);

		if (line == "exit")
		{
			game.Exit();
			break;
		}

		if (line.empty() || line.size() < 2)
			continue; // Re-try, invalid input

		const int column = toupper(line[0]) - 'A'; // Get index of letter relative to first letter of (English) alphabet

		string rowStr = line.substr(1);
		if (rowStr.size() > 2)
			rowStr = rowStr.substr(0, 2);
		try
		{
			game.TakeTurn(stoi(rowStr) - 1, column);
		}
		catch(const exception&) { }
	}

	return 0;
}