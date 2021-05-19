/*
 *
 * AIE Introduction to C++
 * Highscore Database
 * Lewis Comstive (s210314)
 *
 * See the LICENSE file in the root directory of project for copyright.
 *
 */

#pragma once
#include <string>
#include <vector>

#define DATASTREAM_EXCEPTIONS

#define _WRITESTREAM(type, streamType) \
											template<> DataStream* Write< ##type>(##type t) { \
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

	/// Stream of binary, stored in memory using a custom layout
	class DataStream
	{
	private:
		enum class StreamType : char
		{
			Char = 1,
			Int,
			UInt,
			Float,
			Double,
			Bool,
			Long,
			ULong,
			LLong,
			ULLong,
			Short,
			UShort,
			String,
			CharArray
		};

		/// Initial data size, in bytes, to allocate to a stream
		static const int InitialStreamSize;

		char* m_Data;
		bool m_Writing;
		unsigned int m_Index, m_Length;

		void InternalWrite(StreamType streamType, char* data, unsigned int length);
		char* InternalRead(StreamType streamType, unsigned int Length);
		char* InternalReadArray(StreamType streamType, unsigned int* Length);
	public:
		// --- CONSTRUCTORS --- //

		/// Reads a binary file into the DataStream (reading mode).
		/// If file does not exist, creates empty buffer (writing mode)
		/// \param initialSize Starting size of internal buffer if file does not exist
		DataStream(std::string path, unsigned int initialSize = InitialStreamSize);

		/// Copies another DataStream into this one (reading mode)
		DataStream(DataStream* other);

		/// Copies another DataStream into this one (reading mode)
		DataStream(const DataStream& other);

		/// Copies data into DataStream (reading mode)
		/// \param data Valid char array
		/// \param length Length to read from data
		DataStream(char* GetData, unsigned int Length);

		/// Creates an empty buffer (writing mode)
		/// \param initialSize Initial size of data
		DataStream(unsigned int initialSize = InitialStreamSize);

		/// Copies data into DataStream (reading mode)
		/// \param data Valid char vector
		/// \param length Length to read from data
		DataStream(std::vector<char>& GetData, unsigned int Length = 0);

		~DataStream();

		// --- GETTERS & SETTERS --- //

		/// \return True if there is no data inside the stream
		bool IsEmpty() const;

		/// \return True if the stream is set to reading mode
		bool IsReading() const;

		/// \return True if the stream is set to writing mode
		bool IsWriting() const;

		/// \return True if the stream has more data that can be read
		bool IsAvailable() const;

		/// \return The overall length of data inside the stream
		unsigned int Length() const;

		void SetReading();
		void SetWriting();

		/// \return A copy of the internal buffer
		std::vector<char> GetData();

		/// Gets a copy of the internal buffer, and inserts it into `output`. <br>
		/// If `output` is a nullptr, creates a dynamic array.
		void GetData(char output[]);

		/// Outputs a copy of the internal buffer
		void GetData(std::vector<char>* output);

		// --- WRITE --- //

		/// Write the internal buffer to a file.
		/// \param filepath File to be written to
		void WriteToFile(std::string filepath);

		/// Template for writing to the stream
		template<typename T> DataStream* Write(T t) { throw exception("Tried writing invalid type to stream"); }

		/// Writes to the stream
		/// \param c Array of data
		/// \param Length Length of array
		/// \return This streams
		DataStream* Write(char* c, unsigned int Length) { InternalWrite(StreamType::CharArray, c, Length); return this; }

		/// Writes to the stream
		/// \return This stream
		template<> DataStream* Write<unsigned char>(unsigned char c) { InternalWrite(StreamType::Char, (char*)&c, sizeof(unsigned char)); return this; }

		/// Writes to the stream
		/// \return This stream
		template<> DataStream* Write<bool>(bool b) { char c = b ? '1' : '0'; InternalWrite(StreamType::Bool, &c, sizeof(bool)); return this; }

		/// Writes to the stream
		/// \return This stream
		template<> DataStream* Write<char*>(char* c) { InternalWrite(StreamType::CharArray, c, (unsigned int)strlen((const char*)c)); return this; }

		/// Writes to the stream
		/// \return This stream
		template<> DataStream* Write<const char*>(const char* c) { InternalWrite(StreamType::CharArray, (char*)c, (unsigned int)strlen(c)); return this; }

		/// Writes to the stream
		/// \return This stream
		template<> DataStream* Write<unsigned char*>(unsigned char* c) { InternalWrite(StreamType::CharArray, (char*)c, (unsigned int)strlen((const char*)c)); return this; }

		/// Writes to the stream
		/// \return This stream
		template<> DataStream* Write<std::string>(std::string s) { InternalWrite(StreamType::String, (char*)s.c_str(), (unsigned int)s.size()); return this; }

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(int, StreamType::Int)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(unsigned int, StreamType::UInt)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(float, StreamType::Float)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(double, StreamType::Double)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(long, StreamType::Long)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(unsigned long, StreamType::ULong)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(long long, StreamType::LLong)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(unsigned long long, StreamType::ULLong)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(short, StreamType::Short)

		/// Writes to the stream
		/// \return This stream
		_WRITESTREAM(unsigned short, StreamType::UShort)

		// --- READ --- //

		/// Template for reading from the stream
		template<typename T> T Read() { throw exception("Tried reading invalid type"); }

		/// Template for reading an array from the stream
		template<typename T> T ReadArray(unsigned int* Length) { throw exception("Tried reading invalid type"); }

		/// Reads an array from the stream
		/// \param Length The length read from the array
		/// \return Array of data
		template<> char* ReadArray<char*>(unsigned int* Length)
		{
			if (Length == nullptr) return nullptr;
			return (char*)InternalReadArray(StreamType::CharArray, Length);
		}

		/// Reads an array from the stream
		/// \param Length The length read from the array
		/// \return Array of data
		template<> const char* ReadArray<const char*>(unsigned int* Length)
		{
			if (Length == nullptr) return nullptr;
			return (const char*)InternalReadArray(StreamType::CharArray, Length);
		}

		/// Reads an array from the stream
		/// \param Length The length read from the array
		/// \return Array of data
		template<> unsigned char* ReadArray<unsigned char*>(unsigned int* Length)
		{
			if (Length == nullptr) return nullptr;
			return reinterpret_cast<unsigned char*>(InternalReadArray(StreamType::CharArray, Length));
		}

		template<> unsigned char Read<unsigned char>() { return InternalRead(StreamType::Char, sizeof(unsigned char))[0]; }

		template<> bool Read<bool>() { return InternalRead(StreamType::Bool, sizeof(bool))[0] == (unsigned char)'1'; }

		template<> char* Read<char*>() { unsigned int Length = 0; return ReadArray<char*>(&Length); }

		template<> const char* Read<const char*>() { unsigned int Length = 0; return ReadArray<const char*>(&Length); }

		template<> unsigned char* Read<unsigned char*>() { unsigned int Length = 0; return ReadArray<unsigned char*>(&Length); }

		template<> std::string Read<std::string>()
		{
			unsigned int Length = 0;
			char* GetData = InternalReadArray(StreamType::String, &Length);
			return std::string(GetData, Length);
		}

		_READSTREAM(int, StreamType::Int)
		_READSTREAM(unsigned int, StreamType::UInt)
		_READSTREAM(float, StreamType::Float)
		_READSTREAM(double, StreamType::Double)
		_READSTREAM(long, StreamType::Long)
		_READSTREAM(unsigned long, StreamType::ULong)
		_READSTREAM(long long, StreamType::LLong)
		_READSTREAM(unsigned long long, StreamType::ULLong)
		_READSTREAM(short, StreamType::Short)
		_READSTREAM(unsigned short, StreamType::UShort)
	};
}