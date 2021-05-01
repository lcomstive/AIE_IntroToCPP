#include <raylib.h>
#include <World.hpp>
#include <iostream>

using namespace std;
using namespace CellularAutomata;

World::World(unsigned int width, unsigned int height, unsigned int particleSize) : m_Timer(), m_ParticleSize(particleSize)
{
	// Create 2D array of particles, 1 particle = 1 pixel in this case
	m_Particles = new ParticleType*[width];
	for (unsigned int x = 0; x < width; x++)
	{
		m_Particles[x] = new ParticleType[height];
		for (unsigned int y = 0; y < height; y++)
			m_Particles[x][y] = ParticleType::None;
	}

	InitWindow((m_Width = width) * m_ParticleSize, (m_Height = height) * m_ParticleSize, "Cellular Automata");

	// Rendering storage
	m_Pixels = new Color[width * height];
	for (unsigned int x = 0; x < width; x++)
		for (unsigned int y = 0; y < height; y++)
			m_Pixels[y * width + x] = GOLD;

	m_Image = { 0 };
	m_Image.data = m_Pixels;
	m_Image.width = width;
	m_Image.height = height;
	m_Image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	m_Image.mipmaps = 1;

	m_Texture = LoadTextureFromImage(m_Image);
	
	m_Shader = LoadShader(0, "./passthrough_shader.fs");
	m_ShaderWidthLocation = GetShaderLocation(m_Shader, "windowWidth");
	m_ShaderParticlesLocation = GetShaderLocation(m_Shader, "particles");
}

World::~World() { Destroy(); }

unsigned int World::GetWidth() { return m_Width; }
unsigned int World::GetHeight() { return m_Height; }
unsigned int World::GetParticleSize() { return m_ParticleSize; }
unsigned int World::GetParticleCount() { return m_Width * m_Height; }

void World::Destroy()
{
	delete[] m_Pixels;
	delete[] m_Particles;
	CloseWindow();
}

void World::Run()
{
	auto lastFrame = m_Timer.now();
	while (!WindowShouldClose())
	{
		auto now = m_Timer.now();
		float deltaTime = (float)duration_cast<microseconds>(now - lastFrame).count() / 1000.0f;

		Update(deltaTime);
		Draw();

		lastFrame = now;
	}
}

const unsigned int DrawX = 300, DrawY = 500;
void World::Update(float deltaTime)
{
	// cout << deltaTime << endl;
	if (IsKeyDown(KEY_SPACE) && GetParticle(DrawX, DrawY) == ParticleType::None)
		SetParticle(DrawX, DrawY, ParticleType::Sand);

	for (unsigned int x = 0; x < m_Width; x++)
		for (unsigned int y = 0; y < m_Height; y++)
			ParticleRules[m_Particles[x][y]](this, x, y);
}

void World::Draw()
{
	BeginDrawing();
	ClearBackground(Color { 30, 30, 30, 255 });

	BeginShaderMode(m_Shader);

	SetShaderValue(m_Shader, m_ShaderWidthLocation, &m_Width, SHADER_UNIFORM_INT);

	int* particles = new int[m_Width * m_Height];
	for (int x = 0; x < m_Width; x++)
		for (int y = 0; y < m_Height; y++)
			particles[y * m_Width + x] = (int)m_Particles[x][y];

	SetShaderValueV(m_Shader, m_ShaderParticlesLocation, particles, SHADER_UNIFORM_INT, m_Width * m_Height);

	DrawRectangle(0, 0, m_Width, m_Height, WHITE);
	EndShaderMode();

	delete[] particles;
	EndDrawing();
}

ParticleType World::GetParticle(unsigned int x, unsigned int y)
{
	x %= m_Width;
	if (y >= m_Height)
		y = m_Height - 1;
	return m_Particles[x][y];
}

void World::SetParticle(unsigned int x, unsigned int y, ParticleType type)
{
	if (x < m_Width && y < m_Height)
		m_Particles[x][y] = type;
}

void World::MoveParticle(unsigned int x, unsigned int y, unsigned int newX, unsigned int newY, ParticleType replacingType)
{
	ParticleType movingParticle = GetParticle(x, y);
	SetParticle(newX, newY, movingParticle);
	SetParticle(x, y, replacingType);
}