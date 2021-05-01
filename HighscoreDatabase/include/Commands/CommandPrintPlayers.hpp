#pragma once
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandPrintPlayers : public ConsoleCommand
	{
	private:
		const size_t MaxPrintedPlayers = 250;

	public:
		CommandPrintPlayers(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "print") || StartsWith(input, "players"); }

		std::string GetUsage() { return "print|players (username_contains)"; }
		std::string GetDescription() { return "Prints all users (only checks for usernames that contain username_contains if provided)"; }

		void Execute(std::vector<std::string> arguments)
		{
			std::vector<PlayerData*> players;
			if (arguments.size() == 0)
				players = GetDatabase()->GetPlayers();
			else
				players = GetDatabase()->GetPlayers(arguments[0]);

			std::cout << "Players: (" << players.size() << ")" << std::endl;
			size_t startIndex = 0;
			if (startIndex > MaxPrintedPlayers)
			{
				std::cout << "[Showing the " << MaxPrintedPlayers << " last players]" << std::endl;
				startIndex -= MaxPrintedPlayers;
			}

			for(size_t i = startIndex; i < players.size(); i++)
				std::cout << " - " << *players[i] << std::endl;
			std::cout << std::endl;
		}
	};
}