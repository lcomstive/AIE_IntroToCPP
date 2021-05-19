/*
 *
 * AIE Introduction to C++
 * Highscore Database
 * Lewis Comstive (s210314)
 *
 * See the LICENSE file in the root directory of project for copyright.
 *
 */

#pragma once
#include <vector>
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommand.hpp>

namespace HighscoreDatabase
{
	class CommandDatabaseRefresh : public ConsoleCommand
	{
	public:
		CommandDatabaseRefresh(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "refresh"); }

		std::string GetUsage() override { return "refresh"; }
		std::string GetDescription() override { return "Re-reads the player database file and overwrites current player database"; }

		void Execute(std::vector<std::string> arguments) override
		{
			GetDatabase()->Refresh();
			std::cout << "Refreshed players from file" << std::endl;
		}
	};
}