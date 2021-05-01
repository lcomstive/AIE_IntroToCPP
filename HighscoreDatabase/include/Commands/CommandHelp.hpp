#pragma once
#include <vector>
#include <iostream>
#include <Utilities.hpp>
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandHelp : public ConsoleCommand
	{
	private:
		std::vector<ConsoleCommand*>* m_Commands;

	public:
		CommandHelp(PlayerDatabase* db, std::vector<ConsoleCommand*>* commands) : ConsoleCommand(db), m_Commands(commands) { }

		bool CheckCommand(std::string& input) { return StartsWith(input, "help") || StartsWith(input, "?"); }

		std::string GetUsage() { return "help"; }
		std::string GetDescription() { return "Shows all available commands"; }

		void Execute(std::vector<std::string> arguments)
		{
			std::cout << "Usage parameters: <> are required, () are required" << std::endl << std::endl;

			std::cout << "Available commands:" << std::endl;
			for (size_t i = 0; i < m_Commands->size(); i++)
				std::cout << " " << m_Commands->at(i)->GetUsage() << std::endl << "\t" << m_Commands->at(i)->GetDescription() << std::endl;
		}
	};
}