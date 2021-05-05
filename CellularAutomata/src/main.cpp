#include <World.hpp>

using namespace CellularAutomata;

int main(int argc, char* argv[])
{
	Vector2 worldSize  = { 100, 100 };
	Vector2 screenSize = { 800, 600 };
	World world(worldSize, screenSize);

	for (unsigned int x = 25; x < 75; x++)
		for (unsigned int y = 10; y < 40; y++)
			world.SetParticle(x, y, ParticleType::Sand);

	for (unsigned int x = 40; x < 60; x++)
		for (unsigned int y = 50; y < 55; y++)
			world.SetParticle(x, y, ParticleType::Water);

	world.Run();

	return 0;
}