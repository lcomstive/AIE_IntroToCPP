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
#include <string>
#include <vector>
#include <PlayerData.hpp>

namespace HighscoreDatabase
{
	// --- String OPERATIONS --- //

	/// Get the input string in lowercase
	std::string ToLowercase(const std::string& input);

	/// Checks if a string starts with a character sequence
	bool StartsWith(std::string& input, const char* contains);

	/// Checks if a string starts with a character sequence
	bool StartsWith(std::string& input, std::string& contains);

	/// Compares strings a and b. <br>
	/// Returns positive if <code>a</code> occurs later in the alphabet, otherwise negative. Returns 0 if both strings are equal
	int CompareStrings(std::string& a, std::string& b);

	/// Splits a string into an array, using the <code>delimiter</code>
	std::vector<std::string> Split(std::string& input, char delimiter);

	/// Splits a string into an array, using the <code>delimiter</code>
	std::vector<std::string> Split(std::string& input, const char* delimiter);

	/// Splits a string into an array, using the <code>delimiter</code>
	std::vector<std::string> Split(std::string& input, std::string& delimiter);

	// --- QUICK SORT --- //

	/// Inline sorts using the QuickSort algorithm to sort players by their username
	/// \param players Array of players
	/// \param high The highest value to include
	void QuickSortUsername(PlayerData* players, int high);

	/// Inline sorts using the QuickSort algorithm to sort players by their username
	/// \param players Array of players
	/// \param high The highest value to include
	/// \param low The lowest value to include
	void QuickSortUsername(PlayerData* players, int low, int high);

	/// Inline sorts using the QuickSort algorithm to sort players by their score
	/// \param players Array of players
	/// \param high The highest value to include
	void QuickSortScore(PlayerData* players, int high);

	/// Inline sorts using the Quick Sort algorithm to sort players by their username
	/// \param players Array of players
	/// \param high The highest value to include
	/// \param low The lowest value to include
	void QuickSortScore(PlayerData* players, int low, int high);

	// --- BINARY SORT --- //

	/// Binary search algorithm to find a player with the given username
	/// \param players Array of players
	/// \param length Length of player array
	/// \param username Username to search for
	/// \return Pointer to found player, or <code>nullptr</code> if player could not be found
	PlayerData* BinarySearchUsername(PlayerData* players, int length, std::string& username);
}