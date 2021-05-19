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
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommand.hpp>

namespace HighscoreDatabase
{
	class CommandRemovePlayer : public ConsoleCommand
	{
	public:
		CommandRemovePlayer(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "remove") || StartsWith(input, "delete"); }

		std::string GetUsage() override { return "remove|delete <username>"; }
		std::string GetDescription() override { return "Removes player from database"; }

		void Execute(std::vector<std::string> arguments) override 
		{
			if (arguments.empty())
			{
				std::cout << GetUsage() << std::endl;
				return;
			}

			PlayerDatabase* db = GetDatabase();
			PlayerData* player = db->GetPlayer(arguments[0]);
			if (!player)
			{
				std::cout << "Could not find player with username '" << arguments[0] << "'" << std::endl;
				return;
			}

			if (db->RemovePlayer(player))
				std::cout << "Successfully removed player" << std::endl;
			else
				std::cout << "Failed to remove player" << std::endl;
		}
	};
}