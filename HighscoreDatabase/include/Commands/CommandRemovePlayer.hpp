#pragma once
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandRemovePlayer : public ConsoleCommand
	{
	public:
		CommandRemovePlayer(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "remove") || StartsWith(input, "delete"); }

		std::string GetUsage() { return "remove|delete <username>"; }
		std::string GetDescription() { return "Removes player from database"; }

		void Execute(std::vector<std::string> arguments)
		{
			if (arguments.size() == 0)
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