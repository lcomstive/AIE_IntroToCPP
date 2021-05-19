#include <Commands/ConsoleCommand.hpp>

using namespace std;
using namespace HighscoreDatabase;

// Console Command
ConsoleCommand::ConsoleCommand(PlayerDatabase* db) : m_Database(db) { }
PlayerDatabase* ConsoleCommand::GetDatabase() const { return m_Database; }
