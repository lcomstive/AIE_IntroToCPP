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
	class CommandDatabaseSave : public ConsoleCommand
	{
	public:
		CommandDatabaseSave(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "save") || StartsWith(input, "write"); }

		std::string GetUsage() override { return "save|write"; }
		std::string GetDescription() override { return "Saves current player database to file"; }

		void Execute(std::vector<std::string> arguments) override
		{
			GetDatabase()->Write();

			std::cout << "Saved players to file" << std::endl;
		}
	};
}