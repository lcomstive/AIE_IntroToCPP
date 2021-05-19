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
	class CommandAddPlayer : public ConsoleCommand
	{
	public:
		CommandAddPlayer(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "add"); }

		std::string GetUsage() override { return "add <username> (score=0)"; }
		std::string GetDescription() override { return "Adds new player to database"; }

		void Execute(std::vector<std::string> arguments) override
		{
			// Check for at least one argument
			if (arguments.empty())
			{
				std::cout << GetUsage() << std::endl;
				return;
			}

			PlayerDatabase* db = GetDatabase();
			
			int score = 0;

			try
			{	// Attempt to parse score as integer
				score = arguments.size() > 1 ? stoi(arguments[1]) : 0;
			} catch(const std::exception&) { }

			PlayerData* player = db->AddPlayer(arguments[0], score);
			
			if (player)
				std::cout << "Added player " << *player << std::endl;
			else
				std::cout << "Failed to add player" << std::endl;
		}
	};
}