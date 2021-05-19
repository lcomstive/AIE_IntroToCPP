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
	class CommandSearchPlayer : public ConsoleCommand
	{
	public:
		CommandSearchPlayer(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "search") || StartsWith(input, "find"); }

		std::string GetUsage() override { return "search|find <username>"; }
		std::string GetDescription() override { return "Finds a player with matching, or containing, username"; }

		void Execute(std::vector<std::string> arguments) override
		{
			if (arguments.empty())
			{
				std::cout << GetUsage() << std::endl;
				return;
			}

			std::vector<PlayerData*> players = GetDatabase()->GetPlayers(arguments[0]);
			if (!players.empty())
			{
				std::cout << "Found " << players.size() << " players" << std::endl;
				for(PlayerData* player : players)
					std::cout << " - " << *player << std::endl;
			}
			else
				std::cerr << "Failed to find player with username '" << arguments[0] << "'" << std::endl;
			std::cout << std::endl;
		}
	};
}