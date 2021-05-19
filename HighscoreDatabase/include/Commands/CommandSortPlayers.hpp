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
	class CommandSortPlayers : public ConsoleCommand
	{
	public:
		explicit CommandSortPlayers(PlayerDatabase* db) : ConsoleCommand(db) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "sort"); }

		std::string GetUsage() override { return "sort <score|username>"; }
		std::string GetDescription() override { return "Sorts the players by either name or username"; }

		void Execute(std::vector<std::string> arguments) override
		{
			if (arguments.empty())
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