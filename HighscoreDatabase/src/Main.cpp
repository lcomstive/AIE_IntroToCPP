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

const int ConsolePrintMaxPlayers = 500; // Console will only print up to this amount of players
const int RandomScoreMin = 10, RandomScoreMax = 99;
const vector<string> RandomUsernames =
{
	"Lewis",
	"Marcus",
	"James",
	"Terrell",
	"Lemon Man",
	"Avocado",
	"Ryz",
	"Klemze",
	"Ricky",
	"Ryzer",
	"Squidzer",
	"Squidbomb",
	"Mason",
	"AHHHHHHHHHHHHHHH",
	"blue6402",
	"JANO",
	"JONO",
	"Marc",
	"Poggers",
	"Poogers",
	"Boogers"
};

int RandomModifier = 0;
int GetRandomScore()
{
	srand((unsigned int)time(NULL) * RandomModifier++); // Yay pseudorandomness
	return rand() % RandomScoreMax + RandomScoreMin;
}
string GetRandomUsername()
{
	srand((unsigned int)time(NULL) * RandomModifier++); // Yay pseudorandomness
	return RandomUsernames[rand() % RandomUsernames.size()];
}

int main(int argc, char** argv)
{
	bool shouldExit = false;
	srand((unsigned int)time(NULL)); // Random sed
	PlayerDatabase db(GetArgument(argc, argv, "--file"));

	// Add commands
	vector<ConsoleCommand*> commands;

	commands.push_back(new CommandAddPlayer(&db));
	commands.push_back(new CommandSortPlayers(&db));
	commands.push_back(new CommandRemovePlayer(&db));
	commands.push_back(new CommandPrintPlayers(&db));
	commands.push_back(new CommandSearchPlayer(&db));
	commands.push_back(new CommandDatabaseSave(&db));
	commands.push_back(new CommandDatabaseRefresh(&db));

	commands.push_back(new CommandHelp(&db, &commands));
	commands.push_back(new CommandExit(&db, &shouldExit));

	// Initialize database if empty
	if (db.GetPlayerCount() == 0)
		for (int i = 0; i < (int)round(MaxPlayers * 0.75); i++)
			db.AddPlayer(GetRandomUsername(), GetRandomScore());

	vector<PlayerData*> createdPlayers;

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