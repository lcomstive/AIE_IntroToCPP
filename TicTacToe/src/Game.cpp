#include <iostream>

#include "Game.hpp"
#include "Console.hpp"

#ifdef _WIN32
#include <conio.h>
#define CaptureCharacter _getch
#elif __unix__
#define CaptureCharacter getch
#else
#define CaptureCharacter 
#endif

using namespace std;
using namespace TicTacToe;

const int AITurnSpeed = 200; // Milliseconds

struct PlayerCharacter
{
public:
	char Symbol;
	ConsoleColour Colour;
};

const vector<PlayerCharacter> PlayerChars = {
	{ 'X', ConsoleColour::Red	  },
	{ 'O', ConsoleColour::Blue    },
	{ '@', ConsoleColour::Cyan	  },
	{ '*', ConsoleColour::Magenta },
	{ '+', ConsoleColour::Green	  }
};

unsigned int Clamp(unsigned int value, unsigned int min, unsigned int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

Game::Game(GameArgs& args) :
	m_Running(true),
	m_DetailText(""),
	m_BoardSize(args.boardSize),
	m_PlayerCount(args.playerCount),
	m_AIPlayerCount(args.aiPlayerCount),
	m_TotalPlayers(args.playerCount + args.aiPlayerCount)
{
	m_Board = nullptr;
	m_BoardSize = Clamp(m_BoardSize, MinBoardSize, MaxBoardSize);
	m_PlayerCount = Clamp(m_PlayerCount, 0, (unsigned int)PlayerChars.size());
	m_AIPlayerCount = Clamp(m_AIPlayerCount, 0, (unsigned int)PlayerChars.size() - m_PlayerCount); // Amount of players & AI cannot exceed total player cound

	// Add human and AI players
	m_PlayerTypes.insert(m_PlayerTypes.end(), m_PlayerCount, PlayerType::Player);
	m_PlayerTypes.insert(m_PlayerTypes.end(), m_AIPlayerCount, PlayerType::AI);

	Reset();
}

Game::~Game() { Exit(); }

void Game::Exit()
{
	m_Running = false;
	if (!m_Board)
		return; // Board wasn't initialized, or is already deleted

	for (unsigned int i = 0; i < m_BoardSize; i++)
		delete[] m_Board[i];
	delete[] m_Board;
	m_Board = nullptr;
}

bool Game::IsRunning() { return m_Running; }

void Game::Reset()
{
	// Delete board if exists
	if (m_Board)
	{
		for (unsigned int i = 0; i < m_BoardSize; i++)
			delete[] m_Board[i];
		delete[] m_Board;
	}

	// Reset vars
	m_CurrentTurn = 0;
	m_DetailText = "";
	m_AvailableSpots = m_BoardSize * m_BoardSize;

	// Create board
	m_Board = new unsigned int* [m_BoardSize];
	for (unsigned int x = 0; x < m_BoardSize; x++)
	{
		m_Board[x] = new unsigned int[m_BoardSize];
		for (unsigned int y = 0; y < m_BoardSize; y++)
			m_Board[x][y] = -1;
	}
}

void Game::Draw()
{
	if (!m_Running)
		return;

	Console::ChangeColour(ConsoleColour::White);
	Console::Clear();

	DrawControls();
	DrawBoard();

	cout << m_DetailText << endl;

	// Input Prompt text
	cout << "Player " << (m_CurrentTurn + 1) << " (";
	Console::ChangeColour(PlayerChars[m_CurrentTurn].Colour);
	cout << PlayerChars[m_CurrentTurn].Symbol;
	Console::ChangeColour(ConsoleColour::White);
	cout << ")" << endl;
	
	// Check player type
	if (m_PlayerTypes[m_CurrentTurn] == PlayerType::Player)
	{
		// Prompt for user input
		Console::ChangeColour(ConsoleColour::Grey);
		cout << " -> ";
		Console::ChangeColour(ConsoleColour::White);
	}
	else
	{
		// Computer turn
		cout << " AI" << endl;

		AITurn();
		Draw();
	}
}

void Game::DrawControls()
{
	cout << "Standard Tic-Tac-Toe Rules" << endl;
	cout << "Input co-ordinates to place your piece, as a letter-number combination (e.g. A2, C3)" << endl << endl;
}

bool Game::SpotAvailable(unsigned int x, unsigned int y)
{
	// Invalid spot
	if (x >= m_BoardSize || y >= m_BoardSize) return false;

	int result = m_Board[x][y];
	return result < 0; // >= 0 indicates player piece
}

void Game::DrawBoard()
{
	if (!m_Board)
		return; // Board was deleted

	// Board text width with padding and other symbols
	unsigned int boardWidth = m_BoardSize * 3 + (m_BoardSize + 1);

	// Draw column names
	// e.g.
	// 
	//   A  B  C  D  E
	//
	cout << "   ";
	Console::ChangeColour(ConsoleColour::DarkYellow);
	for (unsigned int i = 0; i < m_BoardSize; i++)
		cout << "  " << char('A' + i) << " ";
	cout << endl << "   ";
	Console::ChangeColour(ConsoleColour::White);

	// Draw top of board
	// e.g.
	// 
	// ┌───────────────────┐
	//
	for (unsigned int i = 0; i < boardWidth; i++)
	{
		if (i == 0)
			cout << char(218); // ┌
		else if (i == boardWidth - 1)
			cout << char(191); // ┐
		else
			cout << char(196); // ─
	}
	cout << endl;

	// Draw each board row
	// e.g.
	// 
	//  1 │   │ X │ O │
	//  2 │ O │   │ X │
	//
	for (unsigned int x = 0; x < m_BoardSize; x++)
	{
		Console::ChangeColour(ConsoleColour::DarkYellow);
		cout << (x < 9 ? " " : "") << (x + 1);
		Console::ChangeColour(ConsoleColour::White);
		cout << " " << char(179) << " "; // │

		for (unsigned int y = 0; y < m_BoardSize; y++)
		{
			int playerPiece = m_Board[x][y];
			if (playerPiece < 0)
			{
				cout << "  " << char(179) << " "; // │
				continue;
			}

			Console::ChangeColour(PlayerChars[playerPiece].Colour);
			cout << PlayerChars[playerPiece].Symbol;
			Console::ChangeColour(ConsoleColour::White);

			cout << " " << char(179) << " "; // │
		}
		cout << endl;
	}

	// Draw bottom of board
	// e.g.
	// 
	// └───────────────────┘
	//
	cout << "   ";
	for (unsigned int i = 0; i < boardWidth; i++)
	{
		if (i == 0)
			cout << char(192); // └
		else if (i == boardWidth - 1)
			cout << char(217); // ┘
		else
			cout << char(196); // ─
	}
	Console::ChangeColour(ConsoleColour::White);
	cout << endl;
}

int Game::GetBoardSize() { return m_BoardSize; }
int Game::GetPlayerCount() { return m_PlayerCount; }
int Game::GetCurrentPlayer() { return m_CurrentTurn; }
int Game::GetAIPlayerCount() { return m_AIPlayerCount; }
int Game::GetTotalPlayerCount() { return m_TotalPlayers; }

void Game::TakeTurn(unsigned int row, unsigned int column)
{
	// Check valid coordinates
	if (row >= m_BoardSize || column >= m_BoardSize)
	{
		m_DetailText = "Invalid input";
		return;
	}

	// Check spot isn't taken
	if (!SpotAvailable(row, column))
	{
		m_DetailText = "That is already taken";
		return;
	}

	// Let next user know what turn was taken
	m_DetailText = "Previous turn was " + string(1, char('A' + column)) + to_string(row + 1) + " (" + string(1, PlayerChars[m_CurrentTurn].Symbol) + ")";

	// Set piece at coordinate to player
	m_Board[row][column] = m_CurrentTurn;
	m_AvailableSpots--;

	// Check if the player won this turn
	if (CheckWinConditions(m_CurrentTurn))
	{
		DisplayVictoryScreen(m_CurrentTurn);
		return;
	}

	// Didn't win, check for draw
	if (m_AvailableSpots == 0)
	{
		DisplayVictoryScreen(-1); // No winner, draw
		return;
	}

	// Ready for next turn
	m_CurrentTurn++;
	if (m_CurrentTurn >= (m_PlayerCount + m_AIPlayerCount))
		m_CurrentTurn = 0; // All players had their turn, reset
}

void Game::AITurn()
{
	// Simulate the computer 'thinking' before taking its turn
#ifdef _WIN32
	Sleep(AITurnSpeed);
#elif __unix__
	sleep(AITurnSpeed);
#endif

	// Check for any wins if AI places piece
	for (unsigned int x = 0; x < m_BoardSize; x++)
	{
		for (unsigned int y = 0; y < m_BoardSize; y++)
		{
			if (!SpotAvailable(x, y))
				continue;

			if (CheckWinConditions(m_CurrentTurn, x, y))
			{
				// AI will win
				TakeTurn(x, y);
				return;
			}
		}
	}

	// Check for any wins if player puts a piece, and cut them off
	for (unsigned int x = 0; x < m_BoardSize; x++)
	{
		for (unsigned int y = 0; y < m_BoardSize; y++)
		{
			if (!SpotAvailable(x, y))
				continue;

			for (unsigned int i = 0; i < m_TotalPlayers; i++)
			{
				if (i == m_CurrentTurn)
					continue; // Don't check against ourself

				if (CheckWinConditions(i, x, y))
				{
					TakeTurn(x, y);
					return;
				}
			}
		}
	}

	// Select a random piece
	static int randomCounter = 0;
	srand((unsigned int)time(NULL) * randomCounter++); // Yay pseudorandomness

	int randomX = 0, randomY = 0;
	while (!SpotAvailable(randomX = rand() % m_BoardSize, randomY = rand() % m_BoardSize));

	TakeTurn(randomX, randomY);
}

bool Game::CheckWinConditions(unsigned int player, unsigned int checkX, unsigned int checkY)
{
	// Check down
	for (unsigned int x = 0; x < m_BoardSize; x++)
	{
		for (unsigned int y = 0; y < m_BoardSize; y++)
		{
			bool modifiedPiece = checkX < m_BoardSize && checkX == x &&
								 checkY < m_BoardSize && checkY == y;
			if (m_Board[x][y] != player && !modifiedPiece)
				break;
			if (y == m_BoardSize - 1)
				return true;
		}
	}

	// Check across
	for (unsigned int y = 0; y < m_BoardSize; y++)
	{
		for (unsigned int x = 0; x < m_BoardSize; x++)
		{
			bool modifiedPiece = checkX < m_BoardSize && checkX == x &&
								 checkY < m_BoardSize && checkY == y;
			if (m_Board[x][y] != player && !modifiedPiece)
				break;
			if (x == m_BoardSize - 1)
				return true;
		}
	}

	// Check top-left to bottom-right diagonal
	for (unsigned int i = 0; i < m_BoardSize; i++)
	{
		bool modifiedPiece = checkX < m_BoardSize && checkX == i &&
							 checkY < m_BoardSize && checkY == i;
		if (m_Board[i][i] != player && !modifiedPiece)
			break;

		if (i == m_BoardSize - 1)
			return true;
	}

	// Check top-right to bottom-leftdiagonal
	for (unsigned int i = 0; i < m_BoardSize; i++)
	{
		bool modifiedPiece = checkX < m_BoardSize && checkX == (m_BoardSize - i - 1) &&
							 checkY < m_BoardSize && checkY == i;
		int playerPiece = m_Board[m_BoardSize - i - 1][i];
		if (playerPiece != player && !modifiedPiece)
			break;
		if (i == m_BoardSize - 1)
			return true;
	}

	return false;
}

void Game::DisplayVictoryScreen(int winner)
{
	Console::Clear();

	if (winner >= 0)
	{
		cout << endl << "Player " << (winner + 1) << " (";

		Console::ChangeColour(PlayerChars[winner].Colour);
		cout << PlayerChars[winner].Symbol;
		Console::ChangeColour(ConsoleColour::White);

		cout << ") won!" << endl << endl;
	}
	else
		cout << endl << "It's a draw!" << endl << endl;

	DrawBoard();

	cout << "Do you want to play again? (Y/N)" << endl;

	int input = CaptureCharacter();
	if (input == 'y' || input == 'Y')
	{
		// Shift all players right one to mix it up a bit
		vector<PlayerType> typesCopy(m_PlayerTypes);
		for (size_t i = 0; i < typesCopy.size(); i++)
		{
			size_t index = i < (typesCopy.size() - 1) ? (i + 1) : 0;
			m_PlayerTypes[i] = typesCopy[index];
		}

		Reset();
	}
	else
		Exit();
}