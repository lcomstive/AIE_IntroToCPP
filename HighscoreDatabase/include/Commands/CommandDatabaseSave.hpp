#pragma once
#include <vector>
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandDatabaseSave : public ConsoleCommand
	{
	public:
		CommandDatabaseSave(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "save") || StartsWith(input, "write"); }

		std::string GetUsage() { return "save|write"; }
		std::string GetDescription() { return "Saves current player database to file"; }

		void Execute(std::vector<std::string> arguments)
		{
			GetDatabase()->Write();

			std::cout << "Saved players to file" << std::endl;
		}
	};
}