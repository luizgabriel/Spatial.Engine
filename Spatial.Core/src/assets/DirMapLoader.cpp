#include <spatial/assets/DirMapLoader.h>

namespace spatial::assets
{

DirMapLoader::DirMapLoader() : mData()
{
}

DirMapLoader&& DirMapLoader::add(std::string folderName, std::shared_ptr<ResourcesLoader> loader)
{
	mData.emplace(std::move(folderName), std::move(loader));
	return std::move(*this);
}

ResourceData DirMapLoader::load(const std::string_view fileName) const
{
	const auto separator = fileName.find('/');
	const auto rootName = std::string{fileName.begin(), separator};
	const auto it = this->mData.find(rootName);

	if (it != this->mData.end())
	{
		return it->second->load(fileName.substr(separator + 1));
	}

	return std::nullopt;
}

} // namespace spatial::assets