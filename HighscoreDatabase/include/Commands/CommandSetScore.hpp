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
	class CommandSetPlayerScore : public ConsoleCommand
	{
	public:
		CommandSetPlayerScore(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "set") || StartsWith(input, "change"); }

		std::string GetUsage() override { return "set|change <username> <score>"; }
		std::string GetDescription() override { return "Changes a player's score"; }

		void Execute(std::vector<std::string> arguments) override
		{
			if (arguments.empty())
			{
				std::cout << GetUsage() << std::endl;
				return;
			}

			PlayerData* player = GetDatabase()->GetPlayer(arguments[0]);
			if(!player)
			{
				std::cerr << "Failed to find player with username '" << arguments[0] << "'" << std::endl << std::endl;
				return;
			}

			try
			{
				int newScore = stoi(arguments[1]);
				player->Score = newScore; // This alters database version as it is a pointer
				std::cout << "Changed score of " << player->GetUsername() << " to " << newScore << std::endl;
			}
			catch(const std::exception&)
			{
				std::cerr << "Invalid score" << std::endl;
			}
			
			std::cout << std::endl;
		}
	};
}