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
#include <Commands/ConsoleCommand.hpp>

namespace HighscoreDatabase
{
	class CommandExit : public ConsoleCommand
	{
	private:
		bool* m_ShouldExit;

	public:
		CommandExit(PlayerDatabase* db, bool* shouldExit) : ConsoleCommand(db), m_ShouldExit(shouldExit) { }

		bool CheckCommand(std::string& input) override { return input == "exit" || input == "quit"; }

		std::string GetUsage() override { return "exit|quit"; }
		std::string GetDescription() override { return "Stops the program"; }

		void Execute(std::vector<std::string> arguments) override { *m_ShouldExit = true; }
	};
}