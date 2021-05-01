#pragma once
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandSearchPlayer : public ConsoleCommand
	{
	public:
		CommandSearchPlayer(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "search") || StartsWith(input, "find"); }

		std::string GetUsage() { return "search|find <username>"; }
		std::string GetDescription() { return "Finds a player with matching, or containing, username"; }

		void Execute(std::vector<std::string> arguments)
		{
			if (arguments.size() == 0)
			{
				std::cout << GetUsage() << std::endl;
				return;
			}

			PlayerData* player = GetDatabase()->GetPlayer(arguments[0]);
			if (player)
				std::cout << " - " << *player << std::endl;
			else
				std::cerr << "Failed to find player with username '" << arguments[0] << "'" << std::endl;
			std::cout << std::endl;
		}
	};
}