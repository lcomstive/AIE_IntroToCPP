#pragma once
#include <map>
#include <vector>
#include <raylib.h>
#include <functional>

namespace CellularAutomata
{
	enum class ParticleType : char
	{
		None = 0,
		Sand,
		Water
	};

	extern std::map<ParticleType, Color> ParticleColours;
}