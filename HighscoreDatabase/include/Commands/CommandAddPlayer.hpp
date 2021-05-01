#pragma once
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandAddPlayer : public ConsoleCommand
	{
	public:
		CommandAddPlayer(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "add"); }

		std::string GetUsage() { return "add <username> (score=0)"; }
		std::string GetDescription() { return "Adds new player to database"; }

		void Execute(std::vector<std::string> arguments)
		{
			if (arguments.size() == 0)
			{
				std::cout << GetUsage() << std::endl;
				return;
			}

			PlayerDatabase* db = GetDatabase();
			
			int score = 0;

			try
			{	// Attempt to parse score as integer
				score = arguments.size() > 1 ? stoi(arguments[1]) : 0;
			} catch(std::exception) { }

			PlayerData* player = db->AddPlayer(arguments[0], score);
			
			if (player)
				std::cout << "Added player " << *player << std::endl;
			else
				std::cout << "Failed to add player" << std::endl;
		}
	};
}