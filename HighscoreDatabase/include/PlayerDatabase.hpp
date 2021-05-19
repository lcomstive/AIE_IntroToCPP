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
#include <vector>
#include <string>
#include <PlayerData.hpp>

namespace HighscoreDatabase
{
	/// Maximum amount of players in the database
	const int MaxPlayers = 100000;

	/*
	* Some Tests w/ MaxPlayers
	* (RAM usage is doubled when reading & writing save file)
	* 
	* MaxPlayers = 1000		  - ~ <1MB RAM usage,  15KB save file
	* MaxPlayers = 100000	  - ~  3MB RAM usage, 1.5MB save file
	* MaxPlayers = 1000000	  - ~ 20MB RAM usage, 2.3MB save file
	* MaxPlayers = 10000000   - ~200MB RAM usage, 150MB save file
	* MaxPlayers = 1000000000 - ~ 19GB RAM usage, unknown save file size
	*/
	
	class PlayerDatabase
	{
	public:
		PlayerDatabase(std::string filepath = "./database.data");
		~PlayerDatabase();

		/// Reads the file at path, overwriting the current database
		void Refresh();

		/// Writes the current database to path
		void Write();

		/// Searches for a player containing <code>username</code>, returns nullptr if not found
		PlayerData* GetPlayer(std::string username);

		/// The total amount of players in the database
		unsigned int GetPlayerCount() const;

		/// Get all players in the database
		std::vector<PlayerData*> GetPlayers();

		/// Get all players that contain <code>nameContains</code> in their username
		std::vector<PlayerData*> GetPlayers(std::string nameContains);

		/// \returns True if player was successfully removed
		bool RemovePlayer(PlayerData* player);

		/// \returns True i f player was successfully removed
		bool RemovePlayer(std::string username);

		/// \returns Created player, or nullptr if max player count has been reached
		PlayerData* AddPlayer(std::string username, unsigned int score = 0);

		void SortByScore();
		void SortByUsername();

	private:
		std::string m_Path;

		PlayerData* m_Players; // Array
		unsigned int m_PlayerCount;
	};
}