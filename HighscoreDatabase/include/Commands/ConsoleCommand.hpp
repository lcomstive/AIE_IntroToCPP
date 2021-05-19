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
#include <string>
#include <vector>
#include <PlayerDatabase.hpp>

namespace HighscoreDatabase
{
	class ConsoleCommand
	{
	public:
		ConsoleCommand(PlayerDatabase* db);
		virtual ~ConsoleCommand() { }

		/// Check if the user input should run this command
		/// \param input User input, converted to lowercase
		/// \returns True if this command should execute
		virtual bool CheckCommand(std::string& input) = 0;

		/// Description of the command, to be shown in the help command.<br>
		/// e.g. "Sorts players by either name or score"
		virtual std::string GetDescription() = 0;

		/// Usage of the command. <br>
		/// e.g. "sort <score/name>"
		virtual std::string GetUsage() = 0;

		/// Execute the command using the provided arguments
		/// \param arguments User input split by space and passed as a std::vector
		virtual void Execute(std::vector<std::string> arguments) = 0;

	protected:
		/// The current player database used by the application
		PlayerDatabase* GetDatabase() const;

	private:
		PlayerDatabase* m_Database;
	};
}