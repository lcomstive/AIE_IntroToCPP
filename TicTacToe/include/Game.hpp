/*
 *
 * AIE Introduction to C++
 * TicTacToe
 * Lewis Comstive (s210314)
 *
 * See the LICENSE file in the root directory of project for copyright.
 *
 */

#pragma once
#include <string>
#include <vector>

namespace TicTacToe
{
	/// Game parameters to pass during the creation of the Game class
	struct GameArgs
	{
		/// Width, and consequently height, of the game board
		unsigned int BoardSize = 3;

		/// Number of human players
		unsigned int PlayerCount = 2;

		/// Number of computer players
		unsigned int AiPlayerCount = 0;
	};

	/// TicTacToe game state
	class Game
	{
	public:
		const unsigned int MinBoardSize = 3;
		const unsigned int MaxBoardSize = 26; // Limited only by the amount of letters in the English alphabet

		/// --- CONSTRUCTORS --- ///

		Game(const GameArgs& arguments);
		~Game();

		/// --- GETTERS --- ///

		/// Whether the game is currently in progress or not
		bool IsRunning() const;

		/// The board width/height
		unsigned int GetBoardSize() const;

		/// Human player count
		unsigned int GetPlayerCount() const;

		/// Computer player count
		unsigned int GetAiPlayerCount() const;

		/// Human & computer player combined count
		unsigned int GetTotalPlayerCount() const;

		/// ID of player currently taking a turn
		unsigned int GetCurrentPlayer() const;

		/// --- GAME FUNCTIONS --- ///
		
		/// Renders the game state onto the console
		void Draw();

		/// Resets the game state, pieces, and players
		void Reset();

		/// Current player places a piece at position,
		/// the board is then checked for a winner.
		void TakeTurn(unsigned int row, unsigned int column);

		/// True if no player has a piece at <i>(x, y)</i>
		bool SpotAvailable(unsigned int x, unsigned int y);

		/// Frees all resources associated with game state
		void Exit();

	private:
		enum class PlayerType : char { Player, AI };

		/// 2D Array of integers.
		/// Each int representing a Player's piece (their ID), or -1 if no piece is placed
		int** m_Board;

		/// True while game loop is currently executing
		bool m_Running;

		/// Subtext for extra information (errors, last turn taken, etc.)
		std::string m_DetailText;

		/// Overall player count is determined by the size of this vector.
		/// Determines whether player at index is human or computer
		std::vector<PlayerType> m_PlayerTypes;

		/// Which player's turn is it; range <code>0 - m_PlayerTypes.size()</code>
		unsigned int m_CurrentTurn;

		/// Width of board (also height, as board is square)
		unsigned int m_BoardSize;

		/// Number of human players. Equal to number of PlayerType::Player's in m_PlayerTypes
		unsigned int m_PlayerCount;

		/// Number of computer players. Equal to number of PlayerType::AI's in m_PlayerTypes
		unsigned int m_AIPlayerCount;

		/// How many cells don't have any player pieces. When equal to 0 the board is full, often indicating a draw
		unsigned int m_AvailableSpots;

		/// Number of human and computer players combined
		unsigned int m_TotalPlayers;

		void AITurn();
		void DrawBoard(); // Draw board to current position in console
		void DrawControls(); // Draw controls at current position in console
		void DisplayVictoryScreen(int winner); // Shows which player won and prompts user 'Do you want to play again'

		/// Checks if a player has won.
		/// <br><br>
		/// If <code>checkX</code> and <code>checkY</code> are within board bounds,
		/// tests against a modified board state with player piece inserted.
		bool CheckWinConditions(unsigned int player, unsigned int checkX = 99999, unsigned int checkY = 99999);
	};
}