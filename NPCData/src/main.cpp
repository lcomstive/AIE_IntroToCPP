#include <raylib.h>
#include <DataFile.hpp>

const int ScreenWidth = 800;
const int ScreenHeight = 450;
const string DataFilePath = "npc_data.dat";

/// Clamps `value` between `min` and `max`, inclusive
int clamp(int value, int min, int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

int main(int argc, char* argv[])
{
	// --- INITIALISATION --- //
	
	// Create Raylib window, which also initialises OpenGL context
	InitWindow(ScreenWidth, ScreenHeight, "NPC Data Viewer");

	// Read in data from local file
	DataFile data(DataFilePath);
	if(!data.IsValid())
		return -1;

	int currentRecordIndex = 0;
	DataFile::Record* currentRecord = data.GetRecord(currentRecordIndex); // Get pointer to record data
	Texture2D recordTexture = currentRecord->GetTexture();				  // Texture to display

	SetTargetFPS(60); // Limit FPS to save on system resources being hogged

	// --- DISPLAY LOOP --- //
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Get user input
		int newIndex = currentRecordIndex;

		// Check for number keys pressed
		for (int i = KEY_ONE; i < KEY_NINE; i++)
		{
			if (IsKeyPressed(i))
				newIndex = i - KEY_ONE; // Offset from first key (KEY_ONE)
		}
		newIndex += -1 * IsKeyPressed(KEY_LEFT) + IsKeyPressed(KEY_RIGHT);    // -1 for left, 0 for no change, 1 for right
		newIndex = clamp(newIndex, 0, data.GetRecordCount() - 1); // Clamp selected index within appropriate bounds

		if (newIndex != currentRecordIndex) // Check if record index has changed
		{
			currentRecordIndex = newIndex;

			// Acquire new record
			currentRecord = data.GetRecord(newIndex);
			recordTexture = currentRecord->GetTexture();
		}

		// --- DRAW -- //
		BeginDrawing();

		ClearBackground(RAYWHITE);

		// Draw Texture //
		DrawTexture(recordTexture, 300, 50, WHITE);

		// Draw NPC Info 
		DrawText("NAME", 10, 50, 20, LIGHTGRAY);
		DrawText(currentRecord->name.c_str(), 10, 80, 20, LIGHTGRAY);

		DrawText("AGE", 10, 120, 20, LIGHTGRAY);
		DrawText(to_string(currentRecord->age).c_str(), 10, 150, 20, LIGHTGRAY);

		EndDrawing();
	}

	// Cleanup
	delete currentRecord;
	CloseWindow();        // Close window and OpenGL context

	return 0;
}