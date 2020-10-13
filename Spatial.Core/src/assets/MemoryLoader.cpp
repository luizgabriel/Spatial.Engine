#include <spatial/assets/MemoryLoader.h>

namespace spatial::assets
{

MemoryLoader::MemoryLoader() : mData()
{
}

MemoryLoader::MemoryLoader(std::size_t size) : mData(size)
{
}

ResourceData MemoryLoader::load(const std::string_view fileName) const
{
	const auto it = this->mData.find(std::string{fileName});
	if (it != this->mData.end())
	{
		const auto resourceSpan = it->second;
		return std::vector<char>{resourceSpan.first, resourceSpan.first + resourceSpan.second};
	}

	return std::nullopt;
}

MemoryLoader&& MemoryLoader::add(std::string resourceName, std::pair<const char*, std::size_t> resourceData)
{
	mData.emplace(std::move(resourceName), std::move(resourceData));
	return std::move(*this);
}

MemoryLoader::MemoryLoader(std::initializer_list<ValueType> values) : mData(std::move(values))
{
}

} // namespace spatial::assets