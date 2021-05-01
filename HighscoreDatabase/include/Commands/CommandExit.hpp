#pragma once
#include <Commands/ConsoleCommands.hpp>

namespace HighscoreDatabase
{
	class CommandExit : public ConsoleCommand
	{
	private:
		bool* m_ShouldExit;

	public:
		CommandExit(PlayerDatabase* db, bool* shouldExit) : ConsoleCommand(db), m_ShouldExit(shouldExit) { }

		bool CheckCommand(std::string& input) { return input == "exit" || input == "quit"; }

		std::string GetUsage() { return "exit|quit"; }
		std::string GetDescription() { return "Stops the program"; }

		void Execute(std::vector<std::string> arguments) { *m_ShouldExit = true; }
	};
}