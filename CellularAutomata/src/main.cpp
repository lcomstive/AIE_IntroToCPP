#include <World.hpp>

using namespace CellularAutomata;

int main(int argc, char* argv[])
{
	unsigned int width = 800;
	unsigned int height = 600;
	World world(width, height, 1);

	for (unsigned int x = 50; x < 100; x++)
		for (unsigned int y = 50; y < 100; y++)
			world.SetParticle(x, y, ParticleType::Sand);

	world.Run();

	return 0;
}