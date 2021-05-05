#include <World.hpp>
#include <Particles/Particle.hpp>

using namespace std;
using namespace CellularAutomata;

map<ParticleType, Color> CellularAutomata::ParticleColours =
{
	{ ParticleType::None,  Color {   0,   0,   0,   0 } },
	{ ParticleType::Sand,  Color { 235, 235,  73, 255 } },
	{ ParticleType::Water, Color {  59, 121, 163, 255 } }
};

// RULES //
void ProcessParticleNone(World*, unsigned int, unsigned int) { }

void SandRule(World* world, unsigned int x, unsigned int y)
{
	int checkDirection = (rand() % 10) >= 5 ? 1 : -1;
	// int checkDirection = 1;

	// DOWN //
	if(y > 0)
		switch (world->GetParticle(x, y - 1))
		{
		case ParticleType::None:  return world->MoveParticle (x, y, x, y - 1);
		case ParticleType::Water: return world->SwapParticles(x, y, x, y - 1);
		default: break;
		}

	// DOWN-LEFT //
	if(y > 0)
		switch (world->GetParticle(x - checkDirection, y - 1))
		{
		case ParticleType::None:  return world->MoveParticle (x, y, x - checkDirection, y - 1);
		case ParticleType::Water:
		{
			if (world->GetParticle(x + checkDirection, y) != ParticleType::None)
				break;
			world->MoveParticle(x + checkDirection, y - 1, x + checkDirection, y);
			world->MoveParticle(x, y, x + checkDirection, y - 1);
			break;
		}
		default: break;
		}

	// DOWN-RIGHT //
	if(y > 0)
		switch (world->GetParticle(x + checkDirection, y - 1))
		{
		case ParticleType::None:  return world->MoveParticle (x, y, x + checkDirection, y - 1);
		case ParticleType::Water:
		{
			if (world->GetParticle(x + checkDirection, y) != ParticleType::None)
				break;
			world->MoveParticle(x + checkDirection, y - 1, x + checkDirection, y);
			world->MoveParticle(x, y, x + checkDirection, y - 1);
			break;
		}
		default: break;
		}
}

void WaterRule(World* world, unsigned int x, unsigned int y)
{
	// int checkDirection = (rand() % 10) >= 5 ? 1 : -1;
	int checkDirection = 1;

	// CHECK FOR EMPTY DOWN //
	if (y > 0 && world->GetParticle(x, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x, y - 1);

	// CHECK FOR EMPTY DOWN-LEFT //
	if (y > 0 && world->GetParticle(x - checkDirection, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x - checkDirection, y - 1);

	// CHECK FOR EMPTY DOWN-RIGHT //
	if (y > 0 && world->GetParticle(x + checkDirection, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x + checkDirection, y - 1);

	// CHECK FOR EMPTY LEFT //
	if (world->GetParticle(x - checkDirection, y) == ParticleType::None)
		return world->MoveParticle(x, y, x - checkDirection, y);

	// CHECK FOR EMPTY RIGHT //
	if (world->GetParticle(x + checkDirection, y) == ParticleType::None)
		return world->MoveParticle(x, y, x + checkDirection, y);
}

map<ParticleType, function<void(World*, unsigned int, unsigned int)>> CellularAutomata::ParticleRules =
{
	{ ParticleType::None, ProcessParticleNone },
	{ ParticleType::Sand, SandRule },
	{ ParticleType::Water, WaterRule }
};
