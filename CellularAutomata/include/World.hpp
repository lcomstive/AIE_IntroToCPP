#pragma once
#include <chrono>
#include <Particles/Particle.hpp>

using namespace std::chrono;

// #define SHADER_IMPL

namespace CellularAutomata
{
	class World
	{
	public:
		World(Vector2 worldSize, Vector2 windowSize);
		~World();

		void Run();
		void Destroy();

		unsigned int GetWidth();
		unsigned int GetHeight();
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

		void SwapParticles(
			unsigned int x,
			unsigned int y,
			unsigned int newX,
			unsigned int newY
		);

	private:
		ParticleType** m_Particles;
		high_resolution_clock m_Timer;
		unsigned int m_Width, m_Height;
		unsigned int m_ScreenWidth, m_ScreenHeight;

		Color* m_Pixels;
		
		Image m_Image;
		unsigned int m_CurrentFrame;
		std::vector<Texture2D> m_Textures;

		void Update(float deltaTime);
		void Draw();
	};

	extern std::map<ParticleType, std::function<void(World*, unsigned int, unsigned int)>> ParticleRules;
}