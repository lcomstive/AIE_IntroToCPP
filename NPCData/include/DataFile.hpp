#pragma once

#include "raylib.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

/*

Saved Records Format

	int RecordCount			Number of records stored in this file

	Record:
		int imageWidth		Width of Record.image
		int imageHeight		Height of Record.image
		int nameSize		Length of Record.name (should be changed to be fixed-length and null-terminated)
		int ageSize			WILL ALWAYS BE EQUAL TO sizeof(int), RECOMMENDED TO REMOVE!
		char* imageData		Data of Record.image (size equal to imageWidth * imageHeight * sizeof(Color))
		char* name			Value of Record.name
		int	age				Value of Record.age

*/

#include <iostream>

class DataFile
{
public:
	struct Record
	{
	private:
		Texture2D m_Texture;

	public:
		Image image;
		string name;
		int age;

		Record() : image({ 0 }), m_Texture({}), name(""), age(0) { }

		~Record()
		{
			if (image.width <= 0 || image.height <= 0)
				return;
			UnloadTexture(m_Texture);
		}

		void SetImage(Image& i)
		{
			image = i;
			m_Texture = LoadTextureFromImage(image);
		}

		Texture2D GetTexture()
		{
			if (!image.data || image.width <= 0 || image.height <= 0)
				return { }; // `image`, and consequently `m_Texture`, have not been set
			return m_Texture;
		}
	};

private:
	ifstream m_File;
	string m_Filepath;
	unsigned int m_RecordCount;
	vector<unsigned int> m_RecordSizes;

	Record* m_CurrentRecord;
	unsigned int m_CurrentRecordIndex;

	Record* ReadRecord();

	void SkipNextRecord(int count);
	void SkipPreviousRecord(int count);

public:
	DataFile(const string m_Filepath);
	~DataFile();

	unsigned int GetRecordCount();
	unsigned int GetCurrentRecordIndex();

	void SaveCurrentRecord();
	void AddRecord(string imageFilename, string name, int age);

	Record* GetNextRecord();
	Record* GetCurrentRecord();
	Record* GetPreviousRecord();
	Record* GetRecord(unsigned int index);
};

