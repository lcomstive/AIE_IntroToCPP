#include <raylib.h>
#include <World.hpp>
#include <iostream>
#include <thread>

using namespace std;
using namespace CellularAutomata;

const int FramebufferCount = 2;

World::World(Vector2 worldSize, Vector2 windowSize) : m_Timer()
{
	m_Width  = (int)floor(worldSize.x);
	m_Height = (int)floor(worldSize.y);

	m_ScreenWidth  = (int)floor(windowSize.x);
	m_ScreenHeight = (int)floor(windowSize.y);

	// Create 2D array of particles
	m_Particles = new ParticleType*[m_Width];
	for (unsigned int x = 0; x < m_Width; x++)
	{
		m_Particles[x] = new ParticleType[m_Height];
		for (unsigned int y = 0; y < m_Height; y++)
			m_Particles[x][y] = ParticleType::None;
	}

	InitWindow(m_ScreenWidth, m_ScreenHeight, "Cellular Automata");
	SetTargetFPS(30);

	// Rendering storage
	m_Pixels = new Color[m_Width * m_Height];
	Color defaultColour = ParticleColours[ParticleType::None];
	for (unsigned int x = 0; x < m_Width; x++)
		for (unsigned int y = 0; y < m_Height; y++)
			m_Pixels[(y * m_Width) + x] = defaultColour;
	
	m_Image = { };
	m_Image.width = m_Width;
	m_Image.height = m_Height;
	m_Image.format = UNCOMPRESSED_R8G8B8A8;
	m_Image.mipmaps = 1;
	m_Image.data = m_Pixels;

	m_CurrentFrame = 0;
	for (int i = 0; i < FramebufferCount; i++)
		m_Textures.push_back(LoadTextureFromImage(m_Image));
}

World::~World() { Destroy(); }

unsigned int World::GetWidth() { return m_Width; }
unsigned int World::GetHeight() { return m_Height; }
unsigned int World::GetParticleCount() { return m_Width * m_Height; }

void World::Destroy()
{
	if (!m_Pixels)
		return; // Already destroyed

	for(int i = 0; i < FramebufferCount; i++)
		UnloadTexture(m_Textures[i]);
	m_Textures.clear();

	CloseWindow();

	// delete[] m_Pixels;
	delete[] m_Particles;

	m_Pixels = nullptr;
	m_Particles = nullptr;
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

const unsigned int DrawX = 10, DrawY = 10;
void World::Update(float deltaTime)
{
	Vector2 mousePos = GetMousePosition();
	mousePos.x = (mousePos.x / m_ScreenWidth) * m_Width;
	mousePos.y = m_Height - ((mousePos.y / m_ScreenHeight) * m_Height);

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		SetParticle((unsigned int)mousePos.x, (unsigned int)mousePos.y, ParticleType::Sand);
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
		SetParticle((unsigned int)mousePos.x, (unsigned int)mousePos.y, ParticleType::Water);

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
	ClearBackground(Color{ 30, 30, 30, 255 });

	UpdateTexture(m_Textures[m_CurrentFrame], m_Pixels);

	Rectangle src = { 0, 0, (float)m_Width, (float)m_Height };
	Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
	DrawTexturePro(m_Textures[m_CurrentFrame], src, dest, Vector2{ 0, 0 }, 0, WHITE);

	EndDrawing();

	m_CurrentFrame = (m_CurrentFrame + 1) % FramebufferCount;
}

ParticleType World::GetParticle(unsigned int x, unsigned int y)
{
	if (x >= m_Width)  x = m_Width - 1;
	if (y >= m_Height) y = m_Height - 1;
	return m_Particles[x][y];
}

void World::SetParticle(unsigned int x, unsigned int y, ParticleType type)
{
	if (x >= m_Width)  x = m_Width - 1;
	if (y >= m_Height) y = m_Height - 1;

	m_Particles[x][y] = type;
	m_Pixels[(m_Height - y) * m_Width + x] = ParticleColours[type];
}

void World::MoveParticle(unsigned int x, unsigned int y, unsigned int newX, unsigned int newY, ParticleType replacingType)
{
	if (x >= m_Width)  x = m_Width - 1;
	if (y >= m_Height) y = m_Height - 1;

	ParticleType movingParticle = GetParticle(x, y);
	SetParticle(newX, newY, movingParticle);
	SetParticle(x, y, replacingType);
}

void World::SwapParticles(unsigned int x, unsigned int y, unsigned int newX, unsigned int newY)
{
	if (x >= m_Width)  x = m_Width - 1;
	if (y >= m_Height) y = m_Height - 1;

	ParticleType swapType = GetParticle(newX, newY);
	ParticleType movingParticle = GetParticle(x, y);
	SetParticle(newX, newY, movingParticle);
	SetParticle(x, y, swapType);
}