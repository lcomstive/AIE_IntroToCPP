#include "DataFile.hpp"

using namespace std;

DataFile::Record::Record() : image({ 0 }), m_Texture({}), name(), age(0) { }

DataFile::Record::~Record()
{
	if (image.width <= 0 || image.height <= 0)
		return; // Invalid image

	// Free resources
	UnloadTexture(m_Texture);

	if (image.data)
		RL_FREE(image.data);
}

void DataFile::Record::SetImage(Image i)
{
	image = i;
	m_Texture = LoadTextureFromImage(image);
}

Texture2D DataFile::Record::GetTexture()
{
	if (!image.data || image.width <= 0 || image.height <= 0)
		return { }; // `image`, and consequently `m_Texture`, have not been set
	return m_Texture;
}