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
	if (y == world->GetHeight() - 1)
		return; // Resting state if at bottom of world

	// CHECK FOR EMPTY DOWN //
	if (world->GetParticle(x, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x, y - 1);

	// CHECK FOR EMPTY DOWN-LEFT //
	if (world->GetParticle(x - 1, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x - 1, y - 1);

	// CHECK FOR EMPTY DOWN-RIGHT //
	if (world->GetParticle(x + 1, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x + 1, y - 1);
}

void WaterRule(World* world, unsigned int x, unsigned int y)
{
	if (y == world->GetHeight() - 1)
		return; // Resting state if at bottom of world

	// CHECK FOR EMPTY DOWN //
	if (world->GetParticle(x, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x, y - 1);

	// CHECK FOR EMPTY DOWN-LEFT //
	if (world->GetParticle(x - 1, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x - 1, y - 1);

	// CHECK FOR EMPTY DOWN-RIGHT //
	if (world->GetParticle(x + 1, y - 1) == ParticleType::None)
		return world->MoveParticle(x, y, x + 1, y - 1);

	// CHECK FOR EMPTY LEFT //
	if (world->GetParticle(x - 1, y) == ParticleType::None)
		return world->MoveParticle(x, y, x - 1, y);

	// CHECK FOR EMPTY RIGHT //
	if (world->GetParticle(x + 1, y) == ParticleType::None)
		return world->MoveParticle(x, y, x + 1, y);
}

map<ParticleType, function<void(World*, unsigned int, unsigned int)>> CellularAutomata::ParticleRules =
{
	{ ParticleType::None, ProcessParticleNone },
	{ ParticleType::Sand, SandRule },
	{ ParticleType::Water, WaterRule }
};
