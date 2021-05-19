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
#include <vector>
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommand.hpp>

namespace HighscoreDatabase
{
	class CommandHelp : public ConsoleCommand
	{
	private:
		std::vector<ConsoleCommand*>* m_Commands;

	public:
		CommandHelp(PlayerDatabase* db, std::vector<ConsoleCommand*>* commands) : ConsoleCommand(db), m_Commands(commands) { }

		bool CheckCommand(std::string& input) override { return StartsWith(input, "help") || StartsWith(input, "?"); }

		std::string GetUsage() override { return "help"; }
		std::string GetDescription() override { return "Shows all available commands"; }

		void Execute(std::vector<std::string> arguments) override
		{
			std::cout << "Usage parameters: <> are required, () are required" << std::endl << std::endl;

			std::cout << "Available commands:" << std::endl;
			for (size_t i = 0; i < m_Commands->size(); i++)
				std::cout << " " << m_Commands->at(i)->GetUsage() << std::endl << "\t" << m_Commands->at(i)->GetDescription() << std::endl;
		}
	};
}