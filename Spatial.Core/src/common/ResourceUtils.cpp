#include <spatial/common/ResourceUtils.h>

namespace spatial
{

std::ifstream createStreamFromPath(const std::filesystem::path& filePath)
{
	auto stream = std::ifstream{filePath, std::ios_base::in | std::ios_base::binary};
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	return stream;
}

std::vector<char> createBufferFromStream(std::ifstream&& stream)
{
	const auto iterator = std::istreambuf_iterator<char>{stream};
	return std::vector<char>{iterator, {}};
}

}
