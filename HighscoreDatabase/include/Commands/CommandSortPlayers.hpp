#pragma once
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandSortPlayers : public ConsoleCommand
	{
	public:
		CommandSortPlayers(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "sort"); }

		std::string GetUsage() { return "sort <score|username>"; }
		std::string GetDescription() { return "Sorts the players by either name or username"; }

		void Execute(std::vector<std::string> arguments)
		{
			if (arguments.size() == 0)
			{
				std::cout << GetUsage() << std::endl << std::endl;
				return;
			}

			PlayerDatabase* db = GetDatabase();
			std::string arg = ToLowercase(arguments[0]);
			if (StartsWith(arg, "score"))
			{
				db->SortByScore();
				std::cout << "Sorted players by score" << std::endl << std::endl;
			}
			else if (StartsWith(arg, "name") || StartsWith(arg, "username"))
			{
				db->SortByUsername();
				std::cout << "Sorted players by username" << std::endl << std::endl;
			}
			else
				std::cout << GetUsage() << std::endl;			
		}
	};
}