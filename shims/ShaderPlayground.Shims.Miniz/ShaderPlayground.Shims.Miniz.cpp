#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "miniz.h"

static std::vector<char> ReadBinaryFile(const char* fileName)
{
	std::ifstream ifs(fileName, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	return result;
}

static std::vector<unsigned char> CompressMiniz(std::vector<char>& data, int level)
{
	auto bufferSize = mz_compressBound(data.size());
	std::vector<unsigned char> buffer(bufferSize);
	mz_ulong resSize = bufferSize;
	int res = mz_compress2(buffer.data(), &resSize, (const unsigned char*)data.data(), data.size(), level);
	buffer.resize(resSize);
	return buffer;
}

int main(int argc, const char* argv[])
{
	auto compressionLevel = (int)std::stoi(argv[2]);

	auto shaderBytes = ReadBinaryFile(argv[1]);

	auto outputBytes = CompressMiniz(shaderBytes, compressionLevel);

	std::ofstream out(argv[3], std::ios::binary | std::ios::out);
	out.write((char*)outputBytes.data(), outputBytes.size());
	out.close();

	return 0;
}