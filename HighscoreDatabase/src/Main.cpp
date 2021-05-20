#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <functional>
#include <Utilities.hpp>
#include <PlayerDatabase.hpp>

// Console Commands //
#include <Commands/CommandExit.hpp>
#include <Commands/CommandHelp.hpp>
#include <Commands/CommandSearch.hpp>
#include <Commands/CommandSetScore.hpp>
#include <Commands/CommandAddPlayer.hpp>
#include <Commands/CommandSortPlayers.hpp>
#include <Commands/CommandDatabaseSave.hpp>
#include <Commands/CommandPrintPlayers.hpp>
#include <Commands/CommandRemovePlayer.hpp>
#include <Commands/CommandDatabaseRefresh.hpp>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;
using namespace HighscoreDatabase;

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

const int RandomScoreMin = 10, RandomScoreMax = 99;

int RandomModifier = 0;
int GetRandomScore()
{
	srand(static_cast<unsigned>(time(nullptr)) * RandomModifier++); // Yay pseudorandomness
	return rand() % RandomScoreMax + RandomScoreMin;
}

int main(int argc, char** argv)
{
	bool shouldExit = false;
	srand(static_cast<unsigned>(time(nullptr))); // Random sed
	PlayerDatabase db(GetArgument(argc, argv, "--file"));
	
	// Add commands
	vector<ConsoleCommand*> commands;

	commands.push_back(new CommandAddPlayer(&db));
	commands.push_back(new CommandSortPlayers(&db));
	commands.push_back(new CommandRemovePlayer(&db));
	commands.push_back(new CommandPrintPlayers(&db));
	commands.push_back(new CommandSearchPlayer(&db));
	commands.push_back(new CommandDatabaseSave(&db));
	commands.push_back(new CommandSetPlayerScore(&db));
	commands.push_back(new CommandDatabaseRefresh(&db));

	commands.push_back(new CommandHelp(&db, &commands));
	commands.push_back(new CommandExit(&db, &shouldExit));

	cout << "---------------------------" << endl;
	cout << "-- Player Score Database --" << endl;
	cout << "--    Lewis Comstive     --" << endl;
	cout << "---------------------------" << endl;
	cout << endl << "Try 'help' for available commands" << endl;
	cout << " -> ";

	string line;
	while (!shouldExit && getline(cin, line))
	{
		// Convert lines to vector of space-separated words
		vector<string> splits = Split(line, ' ');

		if (splits.size() == 0) // No input, next loop
		{
			cout << " -> ";
			continue;
		}

		// Check and process all commands
		string cmd = ToLowercase(splits[0]);
		for (ConsoleCommand* command : commands)
		{
			if (!command->CheckCommand(cmd))
				continue;

			splits.erase(splits.begin()); // Remove first argument
			command->Execute(splits);
			break;
		}

		cout << " -> ";
	}

	// Delete the commands
	for (ConsoleCommand* command : commands)
		delete command;
	commands.clear();
	
	// Save changes to database
	db.Write();

	return 0; // Graceful exit
}