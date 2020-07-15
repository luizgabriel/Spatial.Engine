#pragma once

#include <entt/core/hashed_string.hpp>
#include <spatial/assets/AssetReadAction.h>
#include <unordered_map>

namespace spatial
{

template <typename BufferType = BasicBufferType>
struct DirMapLoader : public std::unordered_map<entt::hashed_string::hash_type, AssetReadAction<BufferType>>
{
	using Base = std::unordered_map<entt::hashed_string::hash_type, std::function<bool(std::string_view, BufferType)>>;
	using HashedString = entt::hashed_string;
	using HashedStringId = entt::hashed_string::hash_type;

DirMapLoader(std::initializer_list<typename Base::value_type> args) : Base(args)
	{
	}

	bool operator()(const std::string_view fileName, BufferType buffer) const noexcept
	{
		auto separator = fileName.find('/');
		auto rootName = std::string{fileName.begin(), separator};
		auto fileId = entt::hashed_string{rootName.c_str()}.value();
		auto it = this->find(fileId);

		if (it != this->end())
		{
			return it->second(fileName.substr(separator + 1), buffer);
		}

		return false;
	}
};

} // namespace spatial
