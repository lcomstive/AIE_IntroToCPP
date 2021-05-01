#pragma once
#include <vector>
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandDatabaseRefresh : public ConsoleCommand
	{
	public:
		CommandDatabaseRefresh(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "refresh"); }

		std::string GetUsage() { return "refresh"; }
		std::string GetDescription() { return "Reads the player database file and overwrites current player database"; }

		void Execute(std::vector<std::string> arguments)
		{
			GetDatabase()->Refresh();
			std::cout << "Refreshed players from file" << std::endl;
		}
	};
}