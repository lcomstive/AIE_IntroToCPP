#include <string>
#include <iostream>
#include "Game.hpp"
#include "Console.hpp"

using namespace std;
using namespace TicTacToe;

// "Inspired" by https://gist.github.com/plasticbox/3708a6cdfbece8cd224487f9ca9794cd
string GetArgument(int argc, char** argv, const string& option)
{
	for (int i = 0; i < argc; i++)
	{
		string arg = argv[i];
		if (arg.find(option) == 0)
			return arg.substr(option.size() + 1);
	}
	return "";
}

int GetArgumentInt(int argc, char** argv, const string& option)
{
	string raw = GetArgument(argc, argv, option);
	return raw.empty() ? 0 : stoi(raw); // stoi is a C++11 feature
}

GameArgs ParseArguments(int argc, char** argv)
{
	GameArgs args;
	args.boardSize   = GetArgumentInt(argc, argv, "--board-size");
	args.playerCount = GetArgumentInt(argc, argv, "--player-count");
	args.aiPlayerCount = GetArgumentInt(argc, argv, "--ai-players");
	return args;
}

int main(int argc, char** argv)
{
	GameArgs args = ParseArguments(argc, argv);
	if ((args.playerCount + args.aiPlayerCount) < 2)
		args.aiPlayerCount = 2 - args.playerCount; // Make sure there are at least 2 players, even if both are AI

	Game game(args);
	int boardSize = game.GetBoardSize();

	wstring title = L"Tic Tac Toe Game - Lewis Comstive";
	title += L" (" + to_wstring(boardSize) + L"x" + to_wstring(boardSize) + L")";
	title += L" (Players: " + to_wstring(game.GetPlayerCount()) + L")";
	title += L"(AI: " + to_wstring(game.GetAIPlayerCount()) + L")";

	Console::SetTitle(title);

	string previousMove = "";
	while (game.IsRunning())
	{
		game.Draw();

		if (args.playerCount == 0)
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

		int column = (int)toupper(line[0]) - 65; // 'A' = 65, getting the index of the letter

		string rowStr = line.substr(1);
		if (rowStr.size() > 2)
			rowStr = rowStr.substr(0, 2);
		try
		{
			int row = stoi(rowStr) - 1;
			game.TakeTurn(row, column);
		}
		catch(exception e) { }
	}

	return 0;
}