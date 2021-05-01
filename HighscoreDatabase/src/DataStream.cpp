#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <DataStream.hpp>

using namespace std;
using namespace HighscoreDatabase;

const int DataStream::m_InitialStreamSize = 1024;

// CONSTRUCTORS //
DataStream::DataStream(unsigned int initialSize) : m_Writing(true), m_Index(0), m_Length(initialSize), m_Data(new char[initialSize]) { }
DataStream::DataStream(const DataStream& other) : m_Index(0), m_Writing(false)
{
	m_Length = other.Length();
	m_Data = new char[m_Length];
	memcpy(m_Data, other.m_Data, (size_t)m_Length);
}

DataStream::DataStream(DataStream* other) : m_Index(0), m_Writing(false)
{
	m_Length = (unsigned int)other->Length();
	m_Data = new char[m_Length];
	memcpy(m_Data, other->m_Data, (size_t)m_Length);
}

DataStream::DataStream(vector<char>& input, unsigned int Length) : m_Index(0), m_Writing(false)
{
	m_Length = Length > 0 ? Length : (unsigned int)input.size();
	m_Data = new char[m_Length];
	memcpy(m_Data, input.data(), (size_t)m_Length);
}

DataStream::DataStream(char* GetData, unsigned int Length) : m_Index(0), m_Writing(false)
{
	if (Length <= 0)
	{
#if DATASTREAM_EXCEPTIONS
		throw exception("Length is not assigned");
#else
		m_Length = 0;
		m_Data = nullptr;
		return;
#endif
	}

	m_Length = Length > 0 ? Length : (unsigned int)Length;
	m_Data = new char[m_Length];
	memcpy(m_Data, GetData, (size_t)m_Length);
}

DataStream::DataStream(string path, unsigned int initialSize) : m_Index(0), m_Length(0), m_Writing(false)
{
	ifstream input(path, ios::binary | ios::ate); // Open as binary, seek to end of file to get length

	// Failed to open file, set to writing mode
	if (!input)
	{
		SetWriting();
		m_Data = new char[initialSize];

#ifndef NDEBUG // Debug mode
		cout << "[DataStream] File '" << path << "' could not be opened, setting to write mode" << endl;
#endif
		return;
	}

	// Because ios::ate seeks the entire file, tellg() returns the filesize
	unsigned int fileSize = (unsigned int)input.tellg();

	// Set stream position back at beginning
	input.clear();
	input.seekg(0);

	m_Length = fileSize;
	m_Data = new char[m_Length];
	input.read(m_Data, m_Length);

	input.close();


#ifndef NDEBUG // Debug mode
	cout << "[DataStream] Read binary file into datastream - " << path << " (" << (fileSize / 1024.0f) << "KB)" << endl;
#endif
}

DataStream::~DataStream() { delete[] m_Data; }

// GETTERS & SETTERS //
bool DataStream::IsReading() const { return !m_Writing; }
bool DataStream::IsWriting() const { return m_Writing; }
bool DataStream::IsEmpty() const { return Length() == 0; }
unsigned int DataStream::Length() const { return IsWriting() ? m_Index : m_Length; }
bool DataStream::IsAvailable() const { return IsWriting() ? false : m_Index < (m_Length - 1); }

void DataStream::SetReading()
{
	m_Writing = false;
	m_Length = m_Index;
	m_Index = 0;
}

void DataStream::SetWriting()
{
	m_Writing = true;
	m_Index = 0;
}

std::vector<char> DataStream::GetData()
{
	std::vector<char> d = std::vector<char>();
	GetData(&d);
	return d;
}

void DataStream::GetData(std::vector<char>* output)
{
	if (!output)
		return;
	output->resize(Length());
	if (Length() > 0)
		memcpy(output->data(), m_Data, Length());
}

void DataStream::GetData(char output[])
{
	if (output == nullptr)
		output = new char[Length()];
	memcpy(output, m_Data, (size_t)Length());
}

// READING & WRITING //
void DataStream::WriteToFile(string filepath)
{
	ofstream output(filepath, ios::binary);
	if (!output)
	{
#ifndef NDEBUG // Debug mode
		cerr << "[DataStream] Failed to open file for writing - " << filepath << endl;
#endif
		return;
	}

	// Write data
	output.write(m_Data, Length());

	// Flush data and close file
	output.flush();
	output.close();
}

void DataStream::InternalWrite(StreamType type, char* GetData, unsigned int Length)
{
	if (!m_Writing)
	{
#ifndef NDEBUG
		cerr << "[DataStream] Tried to write to DataStream in reading mode" << endl;
#endif

#if DATASTREAM_EXCEPTIONS
		throw exception("Tried to write to DataStream that was in reading mode");
#else
		return;
#endif
	}

	if ((Length + m_Index + 1) > m_Length)
	{
		char* newData = new char[(size_t)m_Length + m_InitialStreamSize + Length];
		memcpy(newData, m_Data, (size_t)m_Length);
		delete[] m_Data;
		m_Data = newData;
		m_Length += m_InitialStreamSize + Length;
	}

	m_Data[m_Index++] = (unsigned char)type;
	if (type == StreamType::STRING || type == StreamType::CHARARRAY)
	{
		memcpy(m_Data + m_Index, &Length, sizeof(unsigned int));
		m_Index += sizeof(unsigned int);
	}

	memcpy(m_Data + m_Index, GetData, Length);
	m_Index += Length;
}

char* DataStream::InternalRead(StreamType expectedType, unsigned int length) { return InternalReadArray(expectedType, &length); }
char* DataStream::InternalReadArray(StreamType expectedType, unsigned int* length)
{
	if (m_Writing)
	{
#ifndef NDEBUG
		cerr << "[DataStream] Tried reading DataStream while in write mode" << endl;
#endif

#if DATASTREAM_EXCEPTIONS
		throw exception("Tried reading DataStream but is in writing mode");
#else
		return '\0';
#endif
	}

	if (m_Index >= m_Length)
	{
#ifndef NDEBUG
		cerr << "[DataStream] Failed to read - reached end of buffer" << endl;
#endif

#if DATASTREAM_EXCEPTIONS
		throw exception("Failed to read DataStream, reached end of buffer");
#else
		return '\0';
#endif
	}

	StreamType type = (StreamType)m_Data[m_Index++];
	if (type != expectedType)
	{
#ifndef NDEBUG
		cerr << "[DataStream] Read wrong type, aborting..." << endl;
#endif

#if DATASTREAM_EXCEPTIONS
		throw exception("Read wrong type, aborting...");
#else
		return '\0';
#endif
	}

	if (type == StreamType::STRING || type == StreamType::CHARARRAY)
	{
		memcpy(length, m_Data + m_Index, sizeof(unsigned int));
		m_Index += sizeof(unsigned int);
	}

	m_Index += *length;
	return m_Data + m_Index - *length;
}
