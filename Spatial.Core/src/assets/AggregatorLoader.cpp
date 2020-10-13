#include <spatial/assets/AggregatorLoader.h>

namespace spatial::assets
{

AggregatorLoader::AggregatorLoader() : mData()
{
}

AggregatorLoader::AggregatorLoader(std::size_t size) : mData(size)
{
}

ResourceData AggregatorLoader::load(const std::string_view fileName) const
{
	for (auto& disk : mData)
	{
		auto resource = disk->load(fileName);
		if (resource)
			 return resource;
	}

	return std::nullopt;
}

AggregatorLoader&& AggregatorLoader::add(std::shared_ptr<ResourcesLoader> resource)
{
	mData.emplace_back(std::move(resource));
	return std::move(*this);
}

} // namespace spatial::assets