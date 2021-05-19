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
	class CommandPrintPlayers : public ConsoleCommand
	{
	private:
		const size_t MaxPrintedPlayers = 250;

	public:
		CommandPrintPlayers(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "print") || StartsWith(input, "players"); }

		std::string GetUsage() override { return "print|players (username_contains)"; }
		std::string GetDescription() override { return "Prints all users (only checks for usernames that contain username_contains if provided)"; }

		void Execute(std::vector<std::string> arguments) override
		{
			// Get all players
			std::vector<PlayerData*> players;
			if (arguments.empty())
				players = GetDatabase()->GetPlayers();
			else
				players = GetDatabase()->GetPlayers(arguments[0]); // All players w/ username containing string

			std::cout << "Players: (" << players.size() << ")" << std::endl;

			// Check if too many players to print
			if(players.size() > MaxPrintedPlayers)
			{
				std::cout << "[Showing the " << MaxPrintedPlayers << " last players]" << std::endl;

				// Trim array by erasing elements that won't be printed
				players.erase(players.begin(), players.end() - MaxPrintedPlayers);
			}

			for(PlayerData* player : players)
				std::cout << " - " << *player << std::endl;
			std::cout << std::endl;
		}
	};
}