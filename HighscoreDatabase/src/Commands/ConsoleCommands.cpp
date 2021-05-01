#include <Commands/ConsoleCommands.hpp>

using namespace std;
using namespace HighscoreDatabase;

// Console Command
ConsoleCommand::ConsoleCommand(PlayerDatabase* db) : m_Database(db) { }
PlayerDatabase* ConsoleCommand::GetDatabase() { return m_Database; }
