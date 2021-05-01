#pragma once
#include <string>
#include <vector>

namespace TicTacToe
{
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

		Game(GameArgs& arguments);
		~Game();

		// GETTERS //
		bool IsRunning();
		int GetBoardSize();
		int GetPlayerCount();
		int GetAIPlayerCount();
		int GetCurrentPlayer();

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
		enum class PlayerType { Player, AI };

		unsigned int** m_Board;
		bool m_Running;
		std::string m_DetailText;
		std::vector<PlayerType> m_PlayerTypes;
		unsigned int m_CurrentTurn,
			m_PlayerCount,
			m_AIPlayerCount,
			m_BoardSize,
			m_AvailableSpots;

		void AITurn();
		void DrawBoard();
		void DrawControls();
		bool CheckWinConditions(unsigned int player, unsigned int checkX = 99999, unsigned int checkY = 99999);
		void DisplayVictoryScreen(int winner);
	};
}