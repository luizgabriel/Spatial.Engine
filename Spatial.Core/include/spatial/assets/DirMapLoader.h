#pragma once

#include <spatial/assets/ResourcesLoader.h>
#include <string>
#include <unordered_map>

namespace spatial
{

template <typename Action = assets::ResourcesLoader>
struct DirMapLoader : public std::unordered_map<std::string, Action>
{
	using Base = std::unordered_map<std::string, Action>;
	using HashedString = entt::hashed_string;

	DirMapLoader(std::initializer_list<typename Base::value_type> args) : Base(args)
	{
	}

	assets::Resource operator()(const std::string_view fileName) const noexcept
	{
		auto separator = fileName.find('/');
		auto rootName = std::string{fileName.begin(), separator};
		auto it = this->find(rootName);

		if (it != this->end())
		{
			return it->second(fileName.substr(separator + 1));
		}

		return std::nullopt;
	}
};

} // namespace spatial
