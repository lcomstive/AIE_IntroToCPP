#include <iostream>

#include "Game.hpp"
#include "Console.hpp"

#ifdef _WIN32
#include <conio.h> // Console I/O
#define CaptureCharacter _getch
#elif __unix__ || __APPLE__
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>

#define CaptureCharacter getchar
#else
#define CaptureCharacter getch
#endif

using namespace std;
using namespace TicTacToe;

#ifdef TTT_UTF8_SUPPORT
// BARS //
const string BoardVerticalBar   = u8"\u2502"; // │
const string BoardHorizontalBar = u8"\u2500"; // ─

// TOP //
const string BoardTopLeft       = u8"\u250c"; // ┌
const string BoardTopRight      = u8"\u2510"; // ┐

// BOTTOM //
const string BoardBottomLeft    = u8"\u2514"; // └
const string BoardBottomRight   = u8"\u2518"; // ┘
#else
// BARS //
const string BoardVerticalBar   = "|"; // │
const string BoardHorizontalBar = "-"; // ─

// TOP //
const string BoardTopLeft       = "+"; // ┌
const string BoardTopRight      = "+"; // ┐

// BOTTOM //
const string BoardBottomLeft    = "+"; // └
const string BoardBottomRight   = "+"; // ┘
#endif

/// Computer player simulated thinking time, in milliseconds
const int AiTurnSpeed = 200;

/// Player piece data
struct PlayerCharacter
{
	string Symbol;
	ConsoleColour Colour;
};

static const vector<PlayerCharacter> PlayerPieces = {
#ifdef TTT_UTF8_SUPPORT
	{ "X"       , ConsoleColour::Red     }, // X (Cross)
	{ "O"       , ConsoleColour::Blue    }, // O (Nought)
	{ u8"\u002b", ConsoleColour::Cyan    }, // + (Addition Sign)
	{ u8"\u25ca", ConsoleColour::Yellow  }, // ◊ (Lozenge)
	{ u8"\u25cb", ConsoleColour::Green   }  // ○ (Circle)
#else
	{ "X", ConsoleColour::Red     },
	{ "O", ConsoleColour::Blue    },
	{ "@", ConsoleColour::Cyan    },
	{ "*", ConsoleColour::Magenta },
	{ "+", ConsoleColour::Green   }
#endif
};

static const ConsoleColour RowsColour = ConsoleColour::Yellow;
static const ConsoleColour ColumnsColour = ConsoleColour::Yellow;

/// Constrains a value between <code>min</code> and <code>max</code>
unsigned int Clamp(unsigned int value, unsigned int min, unsigned int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

Game::Game(const GameArgs& args) :
	m_Running(true),
	m_CurrentTurn(0),
	m_BoardSize(args.BoardSize),
	m_PlayerCount(args.PlayerCount),
	m_AIPlayerCount(args.AiPlayerCount),
	m_TotalPlayers(args.PlayerCount + args.AiPlayerCount)
{
	m_Board = nullptr; // Gets initialised in Reset()
	m_BoardSize = Clamp(m_BoardSize, MinBoardSize, MaxBoardSize);
	m_PlayerCount = Clamp(m_PlayerCount, 0, static_cast<unsigned int>(PlayerPieces.size()));
	m_AIPlayerCount = Clamp(m_AIPlayerCount, 0, static_cast<unsigned int>(PlayerPieces.size()) - m_PlayerCount); // Amount of players & AI cannot exceed total player count

	// Add human and AI players
	m_PlayerTypes.insert(m_PlayerTypes.end(), m_PlayerCount, PlayerType::Player);
	m_PlayerTypes.insert(m_PlayerTypes.end(), m_AIPlayerCount, PlayerType::AI);

	m_AvailableSpots = m_BoardSize * m_BoardSize;
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

bool Game::IsRunning() const { return m_Running; }

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

	// Create game board, setting all pieces to -1 to represent empty cells
	m_Board = new int* [m_BoardSize];
	for (unsigned int x = 0; x < m_BoardSize; x++)
	{
		m_Board[x] = new int[m_BoardSize];
		for (unsigned int y = 0; y < m_BoardSize; y++)
			m_Board[x][y] = -1;
	}
}

void Game::Draw()
{
	if (!m_Running)
		return;

	Console::Clear();

	DrawControls();
	DrawBoard();

	Console::WriteLine(m_DetailText);

	// Input Prompt text
	Console::Write("Player " + to_string(m_CurrentTurn + 1) + "(");
	Console::Write(PlayerPieces[m_CurrentTurn].Symbol, PlayerPieces[m_CurrentTurn].Colour);
	Console::WriteLine(")");

	// Check player type
	if (m_PlayerTypes[m_CurrentTurn] == PlayerType::Player)
		// Prompt for user input
		Console::Write(" -> ", ConsoleColour::Grey);
	else
	{
		// Computer turn
		Console::WriteLine("AI", ConsoleColour::Grey);

		AITurn();
		Draw();
	}
}

void Game::DrawControls()
{
	Console::WriteLine("Standard Tic-Tac-Toe Rules", ConsoleColour::Grey);
	Console::WriteLine("Input co-ordinates to place your piece, as a letter-number combination (e.g. A2, C3)\n", ConsoleColour::Grey);
}

bool Game::SpotAvailable(unsigned int x, unsigned int y)
{
	// Check invalid spot
	if (x >= m_BoardSize || y >= m_BoardSize) return false;

	int result = m_Board[x][y];
	return result < 0; // >= 0 indicates player piece
}

void Game::DrawBoard()
{
	if (!m_Board)
		return; // Board was deleted

	// Overall board text width, with padding and other symbols
	unsigned int boardWidth = m_BoardSize * 3 + (m_BoardSize + 1);

	// Draw column names
	// e.g.
	// 
	//   A  B  C  D  E
	//
	Console::Write("   ");
	for (unsigned int i = 0; i < m_BoardSize; i++)
	{
		Console::Write("  ");
		Console::Write(char('A' + i), ColumnsColour);
		Console::Write(" ");
	}
	Console::Write("\n   ");

	// Draw top of board
	// e.g.
	//
	// ┌───────────────────┐
	//
	Console::Write(BoardTopLeft);
	for (unsigned int i = 1; i < boardWidth - 1; i++)
		Console::Write(BoardHorizontalBar);
	Console::WriteLine(BoardTopRight);

	// Draw each board row
	// e.g.
	// 
	//  1 │   │ X │ O │
	//  2 │ O │   │ X │
	//
	for (unsigned int x = 0; x < m_BoardSize; x++)
	{
		if(x < 9) Console::Write(" "); // Add whitespace for numbers under 10
		Console::Write(to_string(x + 1), RowsColour);
		Console::Write(" " + BoardVerticalBar + " ");

		for (unsigned int y = 0; y < m_BoardSize; y++)
		{
			int playerPiece = m_Board[x][y];
			if (playerPiece < 0)
			{
				Console::Write("  " + BoardVerticalBar + " ");
				continue;
			}

			Console::Write(PlayerPieces[playerPiece].Symbol, PlayerPieces[playerPiece].Colour);
			Console::Write(" " + BoardVerticalBar + " ");
		}
		Console::WriteLine();
	}

	// Draw bottom of board
	// e.g.
	// 
	// └───────────────────┘
	//
	Console::Write("   " + BoardBottomLeft);
	for (unsigned int i = 1; i < boardWidth - 1; i++)
		Console::Write(BoardHorizontalBar);
	Console::WriteLine(BoardBottomRight);
}

unsigned int Game::GetBoardSize() const { return m_BoardSize; }
unsigned int Game::GetPlayerCount() const { return m_PlayerCount; }
unsigned int Game::GetCurrentPlayer() const { return m_CurrentTurn; }
unsigned int Game::GetAiPlayerCount() const { return m_AIPlayerCount; }
unsigned int Game::GetTotalPlayerCount() const { return m_TotalPlayers; }

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
	m_DetailText = "Previous turn was " + string(1, char('A' + column)) + to_string(row + 1) + " (" + PlayerPieces[m_CurrentTurn].Symbol + ")";

	// Set piece at coordinate to player
	m_Board[row][column] = (int)m_CurrentTurn;
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
	Sleep(AiTurnSpeed);
#elif __unix__ || __APPLE__
	usleep(AiTurnSpeed * 1000);
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
	srand((unsigned int)time(nullptr) * randomCounter++); // Yay pseudorandomness

	unsigned int randomX = 0, randomY = 0;
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

	// Check top-right to bottom-left diagonal
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
		Console::Write("Player " + to_string(winner + 1) + " (");
		Console::Write(PlayerPieces[winner].Symbol, PlayerPieces[winner].Colour);
		Console::WriteLine(") won!\n");
	}
	else
		Console::WriteLine("\nIt's a draw!\n");

	DrawBoard();

	Console::WriteLine("Do you want to play again? (Y/N)");

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