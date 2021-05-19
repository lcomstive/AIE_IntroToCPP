#include <map>
#include <iostream>
#include <Utilities.hpp>
#include <DataStream.hpp>
#include <PlayerDatabase.hpp>

using namespace std;
using namespace HighscoreDatabase;

PlayerDatabase::PlayerDatabase(string filepath) : m_Path(filepath), m_PlayerCount(0), m_Players(nullptr)
{
	if (m_Path.empty())
		m_Path = "./highscores.data";

	Refresh(); // Read file
}

PlayerDatabase::~PlayerDatabase()
{
	Write();
	delete[] m_Players;
}

void PlayerDatabase::Refresh()
{
	if (m_Players)
		delete[] m_Players;
	m_PlayerCount = 0;
	m_Players = new PlayerData[MaxPlayers];

	DataStream stream(m_Path);

	if (stream.IsWriting())
		return; // File could not be read

	m_PlayerCount = stream.Read<unsigned int>();
	if (m_PlayerCount == 0)
		return; // No scores

	if (m_PlayerCount > MaxPlayers)
		m_PlayerCount = MaxPlayers;

	for (unsigned int i = 0; i < m_PlayerCount; i++)
	{
		m_Players[i] = {}; // Initialise as empty struct
		m_Players[i].Score = stream.Read<unsigned int>();
		m_Players[i].SetUsername(stream.Read<string>());
	}

	// Sort alphabetically
	SortByUsername();

#ifndef NDEBUG
	cout << "[PlayerDatabase] Found " << m_PlayerCount << " player scores" << endl;
#endif
}

void PlayerDatabase::Write()
{
	DataStream stream = DataStream(sizeof(PlayerData) * m_PlayerCount);

	stream.Write(m_PlayerCount);
	for (unsigned int i = 0; i < m_PlayerCount; i++)
	{
		stream.Write(m_Players[i].Score);
		stream.Write(m_Players[i].GetUsername());
	}

	stream.WriteToFile(m_Path);

#ifndef NDEBUG
	cout << "[PlayerDatabase] " << m_PlayerCount << " player scores have been written to '" << m_Path << "'" << endl;
#endif
}

unsigned int PlayerDatabase::GetPlayerCount() const { return m_PlayerCount; }

PlayerData* PlayerDatabase::GetPlayer(string username)
{
	// Sort alphabetically and use a binary search
	SortByUsername();
	PlayerData* player = BinarySearchUsername(m_Players, m_PlayerCount, username);
	if (player)
		return player;

	// Brute force search, for partial usernames
	username = ToLowercase(username);
	for (unsigned int i = 0; i < m_PlayerCount; i++)
		if (ToLowercase(m_Players[i].GetUsername()).find(username) != string::npos)
			return &m_Players[i];

	return nullptr; // No name contains 'username'
}

vector<PlayerData*> PlayerDatabase::GetPlayers()
{
	vector<PlayerData*> players = {};
	if (m_PlayerCount == 0)
		return players;
	players.resize(m_PlayerCount);
	for (unsigned int i = 0; i < m_PlayerCount; i++)
		players[i] = &m_Players[i];
	return players;
}

vector<PlayerData*> PlayerDatabase::GetPlayers(std::string nameContains)
{
	vector<PlayerData*> players;
	for (unsigned int i = 0; i < m_PlayerCount; i++)
		if (m_Players[i].GetUsername().find(nameContains) != string::npos)
			players.push_back(&m_Players[i]);
	return players;
}

PlayerData* PlayerDatabase::AddPlayer(string username, unsigned int score)
{
	if (m_PlayerCount >= MaxPlayers)
	{
		cout << "[PlayerDatabase] Failed to add player - maximum count reached" << endl;
		return nullptr;
	}

	m_Players[m_PlayerCount] = PlayerData(username);
	m_Players[m_PlayerCount].Score = score;

	return &m_Players[m_PlayerCount++];
}

bool PlayerDatabase::RemovePlayer(PlayerData* player)
{
	if (!player)
		return false;

	long long playerIndex = player - m_Players;
	for (long long i = playerIndex + 1; i < m_PlayerCount; i++)
		memcpy(&m_Players[i - 1], &m_Players[i], sizeof(PlayerData));
	m_Players[m_PlayerCount - 1] = {}; // Reset as empty struct

	m_PlayerCount--;
	return true;
}

bool PlayerDatabase::RemovePlayer(string username)
{
	PlayerData* player = GetPlayer(username);
	if (!player)
	{
#ifndef NDEBUG
		cerr << "[PlayerDatabase] Failed to remove player - player with that username was not found" << endl;
#endif
		return false;
	}
	return RemovePlayer(player);
}

void PlayerDatabase::SortByScore() { QuickSortScore(m_Players, m_PlayerCount); }
void PlayerDatabase::SortByUsername() { QuickSortUsername(m_Players, m_PlayerCount); }