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
#include <ostream>

namespace HighscoreDatabase
{
	struct PlayerData
	{
		static const size_t MaxUsernameLength = 16;

	private:
		char Username[MaxUsernameLength];

	public:
		unsigned int Score;

		PlayerData() : Score(0) { SetUsername(""); }
		PlayerData(std::string username) : Score(0) { SetUsername(username); }

		std::string GetUsername() const { return std::string(Username); }

		void SetUsername(std::string username)
		{
			// Get the length to copy from username
			size_t length = username.length();
			if (length >= MaxUsernameLength)
				length = MaxUsernameLength - 1;
			// Copy from username parameter into `Username` field
			memcpy(Username, username.data(), length);

			// Make sure remainder of Username is empty
			memset(Username + length, '\0', MaxUsernameLength - length);
		}
	};

	// username [score]
	inline std::ostream& operator <<(std::ostream& stream, const PlayerData& p)
	{
		stream << p.GetUsername() << " [" << p.Score << "]";
		return stream;
	}
}