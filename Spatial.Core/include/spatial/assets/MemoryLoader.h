#pragma once

#include <entt/core/hashed_string.hpp>
#include <span>
#include <string_view>
#include <unordered_map>

namespace spatial
{

template <typename BufferType = BasicBufferType>
class MemoryLoader : public std::unordered_map<entt::hashed_string::hash_type, std::span<const uint8_t>>
{
  public:
	using HashedString = entt::hashed_string;
	using HashedStringId = entt::hashed_string::hash_type;
	using Base = std::unordered_map<HashedStringId, std::span<const uint8_t>>;
	using ValueType = typename Base::value_type;

	MemoryLoader(std::initializer_list<ValueType> args) : Base(args)
	{
	}

	bool operator()(const std::string_view fileName, BufferType buffer) const noexcept
	{
		const auto resourceId = HashedString{fileName.begin()}.value();
		const auto it = this->find(resourceId);
		if (it != this->end())
		{
			const auto resourceSpan = it->second;
			std::copy(resourceSpan.begin(), resourceSpan.end(), buffer);
			return true;
		}

		return false;
	}
};

} // namespace spatial