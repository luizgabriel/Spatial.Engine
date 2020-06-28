#pragma once

#include <filesystem>
#include <fstream>
#include <array>

namespace spatial
{

std::ifstream createStreamFromPath(const std::filesystem::path& filePath);

std::vector<char> createBufferFromStream(std::ifstream&& stream);

class MemoryBuffer : public std::basic_streambuf<char>
{
public:
	template <size_t N>
	explicit MemoryBuffer(const uint8_t (&arr)[N])
		: MemoryBuffer(arr, N)
	{
	}

	template <size_t N>
	explicit MemoryBuffer(const std::array<uint8_t, N>& arr)
		: MemoryBuffer(arr.data(), arr.size())
	{
	}

	MemoryBuffer(const uint8_t* p, size_t l)
	{
		setg((char*)p, (char*)p, (char*)p + l);
	}
};

class MemoryStream : public std::istream
{
private:
	MemoryBuffer mBuffer;

public:
	using Base = std::istream;

	explicit MemoryStream(MemoryBuffer buffer) : Base(&mBuffer), mBuffer(std::move(buffer))
	{
		rdbuf(&mBuffer);
	}
};

} // namespace spatial
