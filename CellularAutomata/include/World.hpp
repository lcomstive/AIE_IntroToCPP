#pragma once
#include <chrono>
#include <Particles/Particle.hpp>

using namespace std::chrono;

namespace CellularAutomata
{
	class World
	{
	public:
		World(unsigned int width, unsigned int height, unsigned int particleSize = 10);
		~World();

		void Run();
		void Destroy();

		unsigned int GetWidth();
		unsigned int GetHeight();
		unsigned int GetParticleSize();
		unsigned int GetParticleCount();

		ParticleType GetParticle(unsigned int x, unsigned int y);
		void SetParticle(unsigned int x, unsigned int y, ParticleType type);

		/// <summary>
		/// Moves a particle to the desired position
		/// </summary>
		/// <param name="replacingType">What to put where the moving particle was</param>
		void MoveParticle(
			unsigned int x,
			unsigned int y,
			unsigned int newX,
			unsigned int newY,
			ParticleType replacingType = ParticleType::None);

	private:
		ParticleType** m_Particles;
		high_resolution_clock m_Timer;
		unsigned int m_Width, m_Height, m_ParticleSize;

		Image m_Image;
		Color* m_Pixels;
		Shader m_Shader;
		Texture m_Texture;
		int m_ShaderWidthLocation;
		int m_ShaderParticlesLocation;

		void Update(float deltaTime);
		void Draw();
	};

	extern std::map<ParticleType, std::function<void(World*, unsigned int, unsigned int)>> ParticleRules;
}