#include <iostream>
#include "DataFile.hpp"

using namespace std;

DataFile::DataFile(const string path) : m_Filepath(path), m_CurrentRecord(nullptr), m_CurrentRecordIndex(0), m_RecordOffsets()
{
	m_File = ifstream(m_Filepath, ios::binary);

	// Read first integer in file, which represents record count
	m_RecordCount = 0;
	m_File.read((char*)&m_RecordCount, sizeof(int));

	// Read through all records once to get offsets
	for (unsigned int i = 0; i < m_RecordCount; i++)
	{
		// Store the current offset
		m_RecordOffsets.push_back(m_File.tellg());

		// Read passed the next record
		// This method deletes the last read record, so only one is ever stored in memory
		ReadRecord();
	}

	// Back to first
	m_File.seekg(m_RecordOffsets[0]);

	// This method deletes the last read record, so only one is ever stored in memory
	ReadRecord();
}

DataFile::~DataFile()
{
	m_File.close();
}

unsigned int DataFile::GetRecordCount() { return m_RecordCount; }
DataFile::Record* DataFile::GetCurrentRecord() { return m_CurrentRecord; }
unsigned int DataFile::GetCurrentRecordIndex() { return m_CurrentRecordIndex; }

void DataFile::AddRecord(string imageFilename, string name, int age)
{
	Image i = LoadImage(imageFilename.c_str());

	Record* r = new Record;
	r->name = name;
	r->age = age;
	r->SetImage(i);

	// records.push_back(r);
	m_RecordCount++;
}

void DataFile::SaveCurrentRecord()
{

}

/*
void DataFile::Save(string filename)
{
	ofstream outfile(filename, ios::binary);

	int recordCount = records.size();
	outfile.write((char*)&recordCount, sizeof(int));

	for (int i = 0; i < recordCount; i++)
	{		
		Color* imgdata = GetImageData(records[i]->image);
				
		int imageSize = sizeof(Color) * records[i]->image.width * records[i]->image.height;
		int nameSize = records[i]->name.length();
		int ageSize = sizeof(int);

		outfile.write((char*)&records[i]->image.width, sizeof(int));
		outfile.write((char*)&records[i]->image.height, sizeof(int));
		
		outfile.write((char*)&nameSize, sizeof(int));
		outfile.write((char*)&ageSize, sizeof(int));

		outfile.write((char*)imgdata, imageSize);
		outfile.write((char*)records[i]->name.c_str(), nameSize);
		outfile.write((char*)&records[i]->age, ageSize);
	}

	outfile.close();
}
*/

// Copied LoadImageEx from Raylib v3.0.0 texture.c
// Versions of Raylib after v3.0.0 remove LoadImageEx
// in favour of LoadImageFromMemory, which doesn't seem
// to load uncompressed image data.
// 
// 
// Load image from Color array data (RGBA - 32bit)
// NOTE: Creates a copy of pixels data array
Image LoadImageEx(Color* pixels, int width, int height)
{
	Image image = { 0 };
	image.data = NULL;
	image.width = width;
	image.height = height;
	image.mipmaps = 1;
	image.format = UNCOMPRESSED_R8G8B8A8;

	int k = 0;

	image.data = (unsigned char*)RL_MALLOC(image.width * image.height * 4 * sizeof(unsigned char));

	for (int i = 0; i < image.width * image.height * 4; i += 4)
	{
		((unsigned char*)image.data)[i] = pixels[k].r;
		((unsigned char*)image.data)[i + 1] = pixels[k].g;
		((unsigned char*)image.data)[i + 2] = pixels[k].b;
		((unsigned char*)image.data)[i + 3] = pixels[k].a;
		k++;
	}

	return image;
}

DataFile::Record* DataFile::ReadRecord()
{
	if (m_CurrentRecordIndex >= m_RecordCount - 1)
		return m_CurrentRecord; // No more records to be read
	if (m_CurrentRecord)
		delete m_CurrentRecord;

	int imageWidth, imageHeight, nameSize;
	m_CurrentRecord = new Record();

	cout << "Current Record Index: " << m_CurrentRecordIndex << endl;
	cout << "Read Current Position Before: " << m_File.tellg() << endl;

	m_File.read((char*)&imageWidth, sizeof(int));
	m_File.read((char*)&imageHeight, sizeof(int));
	m_File.read((char*)&nameSize, sizeof(int));
	m_File.ignore(sizeof(int));

	int imageSize = sizeof(Color) * imageWidth * imageHeight;
	char* imageRaw = new char[imageSize];
	m_File.read(imageRaw, imageSize);

	m_CurrentRecord->SetImage(LoadImageEx((Color*)imageRaw, imageWidth, imageHeight));

	char* nameRaw = new char[nameSize];
	m_File.read(nameRaw, nameSize);

	m_CurrentRecord->name = string(nameRaw, nameRaw + nameSize);
	m_File.read((char*)&m_CurrentRecord->age, sizeof(int));

	cout << "Read Current Position After: " << m_File.tellg() << endl;

	delete[] imageRaw;
	delete[] nameRaw;

	return m_CurrentRecord;
}

DataFile::Record* DataFile::GetNextRecord() { return ReadRecord(); }

DataFile::Record* DataFile::GetPreviousRecord()
{
	if (m_CurrentRecordIndex == 0)
		return m_CurrentRecord; // No records before current

	m_File.seekg(m_RecordOffsets[m_RecordCount -= 2]);
	return ReadRecord();
}

DataFile::Record* DataFile::GetRecord(unsigned int index)
{
	if (index >= m_RecordCount) index = m_RecordCount - 1; // Last record
	if (index == m_CurrentRecordIndex) return m_CurrentRecord; // No change

	m_File.seekg(m_RecordOffsets[index]);
	m_CurrentRecordIndex = index;
	return ReadRecord();
}