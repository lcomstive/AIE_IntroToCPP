#pragma once
#include <string>
#include <vector>
#include <PlayerData.hpp>

namespace HighscoreDatabase
{
	/// <summary>
	/// Get the input string in lowercase
	/// </summary>
	std::string ToLowercase(const std::string& input);

	/// <summary>
	/// Checks if a string starts with a character sequence
	/// </summary>
	bool StartsWith(std::string& input, const char* contains);

	/// <summary>
	/// Checks if a string starts with a character sequence
	/// </summary>
	bool StartsWith(std::string& input, std::string& contains);

	/// <summary>
	/// Compares strings a and b
	/// </summary>
	/// <returns>Positive if 'a' occurs later in the alphabet, otherwise negative. Returns 0 if both strings are equal</returns>
	int CompareStrings(std::string& a, std::string& b);

	/// <summary>
	/// Splits a string into an array, split at each delimiter
	/// </summary>
	std::vector<std::string> Split(std::string& input, char delimiter);

	/// <summary>
	/// Splits a string into an array, split at each delimiter
	/// </summary>
	std::vector<std::string> Split(std::string& input, const char* delimiter);

	/// <summary>
	/// Splits a string into an array, split at each delimiter
	/// </summary>
	std::vector<std::string> Split(std::string& input, std::string& delimiter);

	/// <summary>
	/// Inline sorts using the QuickSort algorithm to sort players by their username
	/// </summary>
	/// <param name="players">Array of players</param>
	/// <param name="high">The highest value to include</param>
	void QuickSortUsername(PlayerData* players, int high);

	/// <summary>
	/// Inline sorts using the QuickSort algorithm to sort players by their username
	/// </summary>
	/// <param name="players">Array of players</param>
	/// <param name="high">The highest value to include</param>
	/// <param name="low">The lowest value to include</param>
	void QuickSortUsername(PlayerData* players, int low, int high);

	/// <summary>
	/// Inline sorts using the QuickSort algorithm to sort players by their score
	/// </summary>
	/// <param name="players">Array of players</param>
	/// <param name="high">The highest value to include</param>
	void QuickSortScore(PlayerData* players, int high);

	/// <summary>
	/// Inline sorts using the Quick Sort algorithm to sort players by their username
	/// </summary>
	/// <param name="players">Array of players</param>
	/// <param name="high">The highest value to include</param>
	/// <param name="low">The lowest value to include</param>
	void QuickSortScore(PlayerData* players, int low, int high);

	/// <summary>
	/// Binary search algorithm to find a player with the given username
	/// </summary>
	/// <param name="players">Array of players</param>
	/// <param name="length">Length of player array</param>
	/// <param name="username">Username to search for</param>
	/// <returns>Pointer to found player, or nullptr if player could not be found</returns>
	PlayerData* BinarySearchUsername(PlayerData* players, int length, std::string& username);
}