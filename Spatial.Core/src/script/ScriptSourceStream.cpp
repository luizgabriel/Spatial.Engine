#include <spatial/script/ScriptSourceStream.h>

namespace spatial::script
{

ScriptSourceStream::ScriptSourceStream(std::unique_ptr<std::istream>&& stream) : mStream{std::move(stream)}
{
}

size_t ScriptSourceStream::GetMoreData(const uint8_t** src)
{
	if (mStream->fail() || mStream->eof())
		return 0;

	constexpr size_t chunkSize = 512;

	auto* data = new uint8_t[chunkSize];
	mStream->read(reinterpret_cast<char*>(data), chunkSize);
	auto size = mStream->gcount();

	*src = data;
	return size;
}

} // namespace spatial::script