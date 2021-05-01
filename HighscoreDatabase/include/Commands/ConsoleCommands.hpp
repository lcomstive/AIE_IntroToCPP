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

		/// <summary>
		/// Check if the user input should run this command
		/// </summary>
		/// <param name="input">User input, converted to lowercase</param>
		/// <returns>True if this command should execute</returns>
		virtual bool CheckCommand(std::string& input) = 0;

		/// <summary>
		/// Description of the command, to be shown in the help command.
		/// e.g. "Sorts players by either name or score"
		/// </summary>
		virtual std::string GetDescription() = 0;

		/// <summary>
		/// Usage of the command.
		/// e.g. "sort <score/name>"
		/// </summary>
		virtual std::string GetUsage() = 0;

		/// <summary>
		/// Execute the command using the provided arguments
		/// </summary>
		/// <param name="arguments">User input split by space and passed as a std::vector</param>
		virtual void Execute(std::vector<std::string> arguments) = 0;

	protected:
		/// <summary>
		/// The current player database used by the console application
		/// </summary>
		PlayerDatabase* GetDatabase();

	private:
		PlayerDatabase* m_Database;
	};
}