#pragma once
#include <string>
#include <vector>

#define DATASTREAM_EXCEPTIONS 0

#define _WRITESTREAM(type, streamType) \
											template<> DataStream* Write<##type>(##type t) { \
																		char GetData[sizeof(##type)]; \
																		memcpy(GetData, &t, sizeof(##type)); \
																		InternalWrite(streamType, GetData, sizeof(##type)); \
																		return this; }
#define _READSTREAM(type, streamType) template<> ##type Read<##type>() { \
																		##type t; \
																		char* readChars = InternalRead(streamType, sizeof(##type)); \
																		if(readChars == '\0') \
																			return (##type()); \
																		memcpy(&t, readChars, sizeof(##type)); \
																		return t; }

namespace HighscoreDatabase
{
	/*
	* Layout of data in the DataStream class
	*
	* char	DataType - Type of data ahead, aligns with DataStream::StreamType
	* char* Data	 - Data to be read
	*
	*/

	/// <summary>
	/// Stream of binary, stored in memory using a custom layout
	/// </summary>
	class DataStream
	{
	private:
		enum class StreamType : char
		{
			CHAR = 1,
			INT,
			UINT,
			FLOAT,
			DOUBLE,
			BOOL,
			LONG,
			ULONG,
			LLONG,
			ULLONG,
			SHORT,
			USHORT,
			STRING,
			CHARARRAY
		};

		/// <summary>
		/// Initial data size, in bytes, to allocate to a stream
		/// </summary>
		static const int m_InitialStreamSize;

		bool m_Writing;
		unsigned int m_Index, m_Length;
		char* m_Data;

		void InternalWrite(StreamType streamType, char* GetData, unsigned int Length);
		char* InternalRead(StreamType streamType, unsigned int Length);
		char* InternalReadArray(StreamType streamType, unsigned int* Length);
	public:
		/// <summary>
		/// Reads a binary file into the DataStream (reading mode).
		/// If file does not exist, creates empty buffer (writing mode)
		/// </summary>
		/// <param name="initialSize">Initial size of internal buffer if file does not exist</param>
		DataStream(std::string path, unsigned int initialSize = m_InitialStreamSize);

		/// <summary>
		/// Copies another DataStream into this one (reading mode)
		/// </summary>
		DataStream(DataStream* other);
		/// <summary>
		/// Copies another DataStream into this one (reading mode)
		/// </summary>
		DataStream(const DataStream& other);

		/// <summary>
		/// Copies data into DataStream (reading mode)
		/// </summary>
		/// <param name="data">Valid char array</param>
		/// <param name="length">Length to read from data</param>
		DataStream(char* GetData, unsigned int Length);

		/// <summary>
		/// Creates an empty buffer (writing mode)
		/// </summary>
		/// <param name="initialSize">Initial size of data</param>
		DataStream(unsigned int initialSize = m_InitialStreamSize);

		/// <summary>
		/// Copies data into DataStream (reading mode)
		/// </summary>
		/// <param name="data">Valid char vector</param>
		/// <param name="length">Length to read from data</param>
		DataStream(std::vector<char>& GetData, unsigned int Length = 0);

		~DataStream();

		/// <returns>True if there is no data inside the stream</returns>
		bool IsEmpty() const;

		/// <returns>True if the stream is set to reading mode</returns>
		bool IsReading() const;

		/// <returns>True if the stream is set to writing mode</returns>
		bool IsWriting() const;

		/// <returns>True if the stream has more data that can be read</returns>
		bool IsAvailable() const;

		/// <returns>The overall length of data inside the stream</returns>
		unsigned int Length() const;

		void SetReading();
		void SetWriting();

		/// <returns>A copy of the internal buffer</returns>
		std::vector<char> GetData();

		/// <summary>
		/// Gets a copy of the internal buffer, and inserts it into `output`.
		/// If `output` is a nullptr, creates a dynamic array.
		/// </summary>
		void GetData(char output[]);

		/// <returns>A copy of the internal buffer</returns>
		void GetData(std::vector<char>* output);

		/// <summary>
		/// Write the internal buffer to a file.
		/// </summary>
		/// <param name="filepath">File to be written to</param>
		void WriteToFile(std::string filepath);

		/// <summary>
		/// Template for writing to the stream
		/// </summary>
		template<typename T> DataStream* Write(T t) { throw exception("Tried writing invalid type to stream"); }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <param name="c">Array of data</param>
		/// <param name="Length">Length of array</param>
		/// <returns>This datastream</returns>
		DataStream* Write(char* c, unsigned int Length) { InternalWrite(StreamType::CHARARRAY, c, Length); return this; }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		template<> DataStream* Write<unsigned char>(unsigned char c) { InternalWrite(StreamType::CHAR, (char*)&c, sizeof(unsigned char)); return this; }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		template<> DataStream* Write<bool>(bool b) { char c = b ? '1' : '0'; InternalWrite(StreamType::BOOL, &c, sizeof(bool)); return this; }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		template<> DataStream* Write<char*>(char* c) { InternalWrite(StreamType::CHARARRAY, c, (unsigned int)strlen((const char*)c)); return this; }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		template<> DataStream* Write<const char*>(const char* c) { InternalWrite(StreamType::CHARARRAY, (char*)c, (unsigned int)strlen(c)); return this; }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		template<> DataStream* Write<unsigned char*>(unsigned char* c) { InternalWrite(StreamType::CHARARRAY, (char*)c, (unsigned int)strlen((const char*)c)); return this; }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		template<> DataStream* Write<std::string>(std::string s) { InternalWrite(StreamType::STRING, (char*)s.c_str(), (unsigned int)s.size()); return this; }

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(int, StreamType::INT)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(unsigned int, StreamType::UINT)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(float, StreamType::FLOAT)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(double, StreamType::DOUBLE)


		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(long, StreamType::LONG)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(unsigned long, StreamType::ULONG)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(long long, StreamType::LLONG)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(unsigned long long, StreamType::ULLONG)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(short, StreamType::SHORT)

		/// <summary>
		/// Writes to the stream
		/// </summary>
		/// <returns>This datastream</returns>
		_WRITESTREAM(unsigned short, StreamType::USHORT)

		/// <summary>
		/// Template for reading from the stream
		/// </summary>
		template<typename T> T Read() { throw exception("Tried reading invalid type"); }

		/// <summary>
		/// Template for reading an array from the stream
		/// </summary>
		template<typename T> T ReadArray(unsigned int* Length) { throw exception("Tried reading invalid type"); }

		/// <summary>
		/// Reads an array from the stream
		/// </summary>
		/// <param name="Length">The length read from the array</param>
		/// <returns>Array of data</returns>
		template<> char* ReadArray<char*>(unsigned int* Length)
		{
			if (Length == nullptr) return nullptr;
			return (char*)InternalReadArray(StreamType::CHARARRAY, Length);
		}

		/// <summary>
		/// Reads an array from the stream
		/// </summary>
		/// <param name="Length">The length read from the array</param>
		/// <returns>Array of data</returns>
		template<> const char* ReadArray<const char*>(unsigned int* Length)
		{
			if (Length == nullptr) return nullptr;
			return (const char*)InternalReadArray(StreamType::CHARARRAY, Length);
		}

		/// <summary>
		/// Reads an array from the stream
		/// </summary>
		/// <param name="Length">The length read from the array</param>
		/// <returns>Array of data</returns>
		template<> unsigned char* ReadArray<unsigned char*>(unsigned int* Length)
		{
			if (Length == nullptr) return nullptr;
			return (unsigned char*)InternalReadArray(StreamType::CHARARRAY, Length);
		}

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		template<> unsigned char Read<unsigned char>() { return InternalRead(StreamType::CHAR, sizeof(unsigned char))[0]; }

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		template<> bool Read<bool>() { return InternalRead(StreamType::BOOL, sizeof(bool))[0] == (unsigned char)'1'; }

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		template<> char* Read<char*>() { unsigned int Length = 0; return ReadArray<char*>(&Length); }

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		template<> const char* Read<const char*>() { unsigned int Length = 0; return ReadArray<const char*>(&Length); }

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		template<> unsigned char* Read<unsigned char*>() { unsigned int Length = 0; return ReadArray<unsigned char*>(&Length); }

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		template<> std::string Read<std::string>()
		{
			unsigned int Length = 0;
			char* GetData = InternalReadArray(StreamType::STRING, &Length);
			return std::string(GetData, Length);
		}

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(int, StreamType::INT)
		
		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(unsigned int, StreamType::UINT)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(float, StreamType::FLOAT)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(double, StreamType::DOUBLE)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(long, StreamType::LONG)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(unsigned long, StreamType::ULONG)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(long long, StreamType::LLONG)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(unsigned long long, StreamType::ULLONG)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(short, StreamType::SHORT)

		/// <summary>
		/// Reads data from the stream
		/// </summary>
		_READSTREAM(unsigned short, StreamType::USHORT)
	};
}