#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <raylib.h>
#include <iostream>

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

class DataFile
{
public:
	struct Record
	{
		Image image;
		string name;
		int age;

		Record();
		~Record();

		Texture2D GetTexture();
		void SetImage(Image& i);

	private:
		Texture2D m_Texture;
	};

private:
	ifstream m_File;
	string m_Filepath;

	Record* m_CurrentRecord;
	unsigned int m_RecordCount;
	unsigned int m_CurrentRecordIndex;
	vector<streampos> m_RecordOffsets;

	Record* ReadRecord();

public:
	/// Tries to open and read data file
	DataFile(const string m_Filepath);
	~DataFile();

	/// \returns Total records read from file
	unsigned int GetRecordCount();

	/// \returns Index of record that is currently read into memory
	unsigned int GetCurrentRecordIndex();

	void SaveCurrentRecord();
	void AddRecord(const string& imageFilename, const string& name, const int& age);

	Record* GetNextRecord();
	Record* GetCurrentRecord();
	Record* GetPreviousRecord();
	Record* GetRecord(unsigned int index);
};

