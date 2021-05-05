#include <raylib.h>
#include "DataFile.hpp"

int clamp(int value, int min, int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	DataFile data("npc_data.dat");
	int currentRecordIdx = 0;

	DataFile::Record* currentRecord = data.GetRecord(0);
	Texture2D recordTexture = currentRecord->GetTexture();

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Get user input
		int newIndex = currentRecordIdx;
		for (int i = KEY_ONE; i < KEY_NINE; i++)
		{
			if (IsKeyPressed(i))
				newIndex = i - KEY_ONE;
		}
		newIndex += -1 * IsKeyPressed(KEY_LEFT) + IsKeyPressed(KEY_RIGHT); // -1 for left, 0 for no change, 1 for right
		newIndex = clamp(newIndex, 0, data.GetRecordCount() - 1); // Clamp selected index within appropriate bounds

		if (newIndex != currentRecordIdx) // Check if record index has changed
		{
			currentRecordIdx = newIndex;

			// Acquire new record
			currentRecord = data.GetRecord(newIndex);
			recordTexture = currentRecord->GetTexture();
		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawTexture(recordTexture, 300, 50, WHITE);

		DrawText("NAME", 10, 50, 20, LIGHTGRAY);
		DrawText(currentRecord->name.c_str(), 10, 80, 20, LIGHTGRAY);

		DrawText("AGE", 10, 120, 20, LIGHTGRAY);
		DrawText(to_string(currentRecord->age).c_str(), 10, 150, 20, LIGHTGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	delete currentRecord;

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}