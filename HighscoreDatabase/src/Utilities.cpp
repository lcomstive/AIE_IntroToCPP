#include <iostream>
#include <algorithm>
#include <Utilities.hpp>

using namespace std;
using namespace HighscoreDatabase;

string HighscoreDatabase::ToLowercase(const std::string& input)
{
	string output(input); // Copy input
	transform(input.begin(), input.end(), output.begin(), [](unsigned char c) { return tolower(c); });
	return output;
}

int HighscoreDatabase::CompareStrings(string& a, string& b)
{
	size_t readLength = a.length();
	if (b.length() < readLength)
		readLength = b.length();
	for (size_t i = 0; i < readLength; i++)
	{
		int value = tolower(a[i]) - tolower(b[i]); // Compare ASCII characters by value
		if (value != 0)
			return value; // Positive if a is larger, or negative if b is larger
	}
	if (a.length() > readLength) return 1;	// A is longer, therefore further alphabetically
	if (b.length() > readLength) return -1; // B is longer, therefore further alphabetically
	return 0; // Same string
}

bool HighscoreDatabase::StartsWith(std::string& input, const char* contains)  { return input.find(contains, 0) == 0; } // Returns true if `contains` is found at index 0
bool HighscoreDatabase::StartsWith(std::string& input, std::string& contains) { return input.find(contains, 0) == 0; } // Returns true if `contains` is found at index 0

// "Influenced" by https://stackoverflow.com/a/14266139
vector<string> HighscoreDatabase::Split(string& input, string& delimiter)
{
	string value(input); // Copy input
	size_t pos = 0;
	vector<string> output; // Store splits
	while ((pos = value.find(delimiter)) != string::npos) // Find next occurrence of delimiter
	{
		output.push_back(value.substr(0, pos));
		value.erase(0, pos + delimiter.length()); // Remove found occurrence from search
	}
	output.push_back(value);

	return output;
}

std::vector<std::string> HighscoreDatabase::Split(std::string& input, char delimiter) { return Split(input, string(1, delimiter)); }
std::vector<std::string> HighscoreDatabase::Split(std::string& input, const char* delimiter) { return Split(input, string(delimiter)); }

/// Swaps values of the pointers
void SwapElements(PlayerData* first, PlayerData* second)
{
	PlayerData temp = *first;
	*first = *second;
	*second = temp;
}

int QuickSortPartitionScore(PlayerData* players, int low, int high)
{
	unsigned int pivot = players[high].Score;
	int index = low - 1;

	for (int j = low; j < high; j++)
	{
		// Check for larger value, then no need to swap
		if (players[j].Score > pivot) continue;

		// Check for if same value, whether username is larger; if so, no need to swap
		if (players[j].Score == pivot && CompareStrings(players[j].GetUsername(), players[high].GetUsername()) > 0) continue;

		// Swap next element
		index++;
		SwapElements(&players[index], &players[j]);
	}

	SwapElements(&players[index + 1], &players[high]);
	return index + 1;
}

int QuickSortPartitionUsername(PlayerData* players, int low, int high)
{
	string pivot = players[high].GetUsername();
	int index = low - 1;

	for (int j = low; j < high; j++)
	{
		// Store comparison value
		int stringComparison = CompareStrings(players[j].GetUsername(), pivot);

		if (stringComparison > 0) continue; // players[j] username is larger, no need to swap

		// Check for if same username, but larger score; if so, no need to swap
		if (stringComparison == 0 && players[j].Score > players[high].Score) continue;

		// Swap next element
		index++;
		SwapElements(&players[index], &players[j]);
	}

	SwapElements(&players[index + 1], &players[high]);
	return index + 1;
}

void HighscoreDatabase::QuickSortUsername(PlayerData* players, int low, int high)
{
	if (low >= high)
		return;
	int pivotIndex = QuickSortPartitionUsername(players, low, high);
	QuickSortUsername(players, low, pivotIndex - 1);
	QuickSortUsername(players, pivotIndex + 1, high);
}

void HighscoreDatabase::QuickSortUsername(PlayerData* players, int high) { QuickSortUsername(players, 0, high - 1); }

void HighscoreDatabase::QuickSortScore(PlayerData* players, int low, int high)
{
	if (low >= high)
		return;
	int pivotIndex = QuickSortPartitionScore(players, low, high);
	QuickSortScore(players, low, pivotIndex - 1);
	QuickSortScore(players, pivotIndex + 1, high);
}

void HighscoreDatabase::QuickSortScore(PlayerData* players, int high) { QuickSortScore(players, 0, high - 1); }

PlayerData* HighscoreDatabase::BinarySearchUsername(PlayerData* players, int length, string& username)
{
	int left = 0;
	int right = length - 1;

	while (left <= right)
	{
		int mid = (left + right) / 2;
		int value = CompareStrings(players[mid].GetUsername(), username);

		if (value == 0)
			return &players[mid]; // Found!

		if (value < 0)
			left = mid + 1;
		else
			right = mid - 1;
	}
	return nullptr;
}