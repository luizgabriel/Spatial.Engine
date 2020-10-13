#pragma once

#include <spatial/assets/ResourcesLoader.h>
#include <string>
#include <unordered_map>

namespace spatial::assets
{

struct DirMapLoader : public ResourcesLoader
{
  private:
	std::unordered_map<std::string, std::shared_ptr<ResourcesLoader>> mData;

  public:
	DirMapLoader();

	DirMapLoader&& add(std::string folderName, std::shared_ptr<ResourcesLoader> loader);

	template <typename LoaderType>
	DirMapLoader&& emplace(std::string folderName, LoaderType&& loader)
	{
		auto sharedLoader = std::make_shared<LoaderType>(std::move(loader));
		return add(std::move(folderName), std::move(sharedLoader));
	}

	template <typename LoaderType, typename... Args>
	DirMapLoader&& emplace(std::string folderName, Args&&... args)
	{
		auto loader = std::make_shared<LoaderType>(std::forward<Args>(args)...);
		return add(std::move(folderName), std::move(loader));
	}

	DirMapLoader(const DirMapLoader& other) = delete;

	DirMapLoader(DirMapLoader&& other) = default;

	ResourceData load(const std::string_view fileName) const override;
};

} // namespace spatial::assets
