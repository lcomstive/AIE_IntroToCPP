#pragma once
#include <string>
#include <vector>

namespace TicTacToe
{
	/// <summary>
	/// Game parameters to pass during the creation of the Game class
	/// </summary>
	struct GameArgs
	{
	public:
		unsigned int boardSize = 3;
		unsigned int playerCount = 2;
		unsigned int aiPlayerCount = 0;
	};

	class Game
	{
	public:
		const unsigned int MinBoardSize = 3;
		const unsigned int MaxBoardSize = 26;

		explicit Game(GameArgs& arguments);
		~Game();

		// GETTERS //
		bool IsRunning() const;

		/// <returns>Board width, same as height</returns>
		unsigned int GetBoardSize() const;

		/// <returns>Human player count</returns>
		unsigned int GetPlayerCount() const;

		/// <returns>AI player count</returns>
		unsigned int GetAIPlayerCount() const;

		/// <returns>Human & AI player count</returns>
		unsigned int GetTotalPlayerCount() const;

		/// <returns>Player taking turn</returns>
		unsigned int GetCurrentPlayer() const;

		// GAME FUNCTIONS //
		
		/// <summary>
		/// Clears the console then renders the game state onto it
		/// </summary>
		void Draw();

		/// <summary>
		/// Resets the board
		/// </summary>
		void Reset();

		/// <summary>
		/// The current player places a piece at the position,
		/// then the board is checked for a winner.
		/// </summary>
		/// <param name="row">Horizontal row</param>
		/// <param name="column">Vertical column</param>
		void TakeTurn(unsigned int row, unsigned int column);

		/// <returns>True if no player has a piece at (x, y)</returns>
		bool SpotAvailable(unsigned int x, unsigned int y);

		/// <summary>
		/// Destroys all resources associated with this game
		/// </summary>
		void Exit();

	private:
		enum class PlayerType : char { Player, AI };

		/// <summary>
		/// 2D Array of integers.
		/// Each int is the player who has placed a piece there, or -1 if no piece is placed
		/// </summary>
		int** m_Board;

		/// <summary>
		/// True while game loop is currently executing
		/// </summary>
		bool m_Running;

		/// <summary>
		/// Subtext for extra information (errors, last turn taken, etc.)
		/// </summary>
		std::string m_DetailText;

		/// <summary>
		/// Player count is determined by the size of this vector.
		/// Determines whether player at vector index is human or computer
		/// </summary>
		std::vector<PlayerType> m_PlayerTypes;

		unsigned int m_BoardSize,		// Width of board (also height, as board is square)
					 m_CurrentTurn,		// Which player's turn is it - corresponds with m_PlayerTypes
					 m_PlayerCount,		// Number of human players. Equal to number of PlayerType::Player's in m_PlayerTypes
					 m_AIPlayerCount,	// Number of computer players. Equal to number of PlayerType::AI's in m_PlayerTypes
					 m_AvailableSpots,	// How many cells are left untouched by a player. When equal to 0 the board is full, often indicating a draw
					 m_TotalPlayers;	// Number of combined human and computer players

		void AITurn();
		void DrawBoard(); // Draw board to current position in console
		void DrawControls(); // Draw controls at current position in console
		void DisplayVictoryScreen(int winner); // Shows which player won and prompts user 'Do you want to play again'

		/// <summary>
		/// Checks if `player` has won the game.
		///
		/// If `checkX` and `checkY` are within board bounds, checks win condition as
		/// if player has a piece there as well.
		/// </summary>
		bool CheckWinConditions(unsigned int player, unsigned int checkX = 99999, unsigned int checkY = 99999);
	};
}