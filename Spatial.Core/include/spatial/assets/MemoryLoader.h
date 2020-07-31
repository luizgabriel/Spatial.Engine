#pragma once

#include <sstream>
#include <string_view>
#include <unordered_map>

namespace spatial
{

class MemoryLoader : public std::unordered_map<std::string, std::pair<const char*, std::size_t>>
{
  public:
	using Base = std::unordered_map<std::string, std::pair<const char*, std::size_t>>;
	using ValueType = typename Base::value_type;

	MemoryLoader(std::initializer_list<ValueType> args) : Base(args)
	{
	}

	assets::Resource operator()(const std::string_view fileName) const noexcept
	{
		const auto it = this->find(std::string{fileName});
		if (it != this->end())
		{
			const auto resourceSpan = it->second;
			return std::vector<char>{resourceSpan.first, resourceSpan.first + resourceSpan.second};
		}

		return std::nullopt;
	}
};

} // namespace spatial