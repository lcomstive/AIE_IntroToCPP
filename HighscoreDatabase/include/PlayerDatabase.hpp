#pragma once
#include <vector>
#include <string>
#include <DataStream.hpp>
#include <PlayerData.hpp>


namespace HighscoreDatabase
{
	/// <summary>
	/// Maximum amount of players in the database
	/// </summary>
	const int MaxPlayers = 1000;

	/*
	* Some Tests w/ MaxPlayers
	* (RAM usage is doubled when reading & writing save file)
	* 
	* MaxPlayers = 1000		- ~ <1MB RAM usage,  15KB save file
	* MaxPlayers = 100000	- ~  3MB RAM usage, 1.5MB save file
	* MaxPlayers = 1000000	- ~ 20MB RAM usage, 2.3MB save file
	* MaxPlayers = 10000000 - ~200MB RAM usage, 150MB save file
	*/
	
	class PlayerDatabase
	{
	public:
		PlayerDatabase(std::string filepath = "./database.data");
		~PlayerDatabase();

		/// <summary>
		/// Reads the file at path, overwriting the current database
		/// </summary>
		void Refresh();

		/// <summary>
		/// Writes the current database to path
		/// </summary>
		void Write();

		PlayerData* GetPlayer(std::string username);
		
		unsigned int GetPlayerCount();
		std::vector<PlayerData*> GetPlayers();
		std::vector<PlayerData*> GetPlayers(std::string nameContains);

		bool RemovePlayer(PlayerData* player);
		bool RemovePlayer(std::string username);
		PlayerData* AddPlayer(std::string username, unsigned int score = 0);

		void SortByScore();
		void SortByUsername();

	private:
		std::string m_Path;

		PlayerData* m_Players;
		unsigned int m_PlayerCount;
	};
}