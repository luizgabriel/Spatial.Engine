#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace spatial
{

class MemoryLoader : public std::unordered_map<std::string, std::string_view>
{
  public:
	using Base = std::unordered_map<std::string, std::string_view>;
	using ValueType = typename Base::value_type;

	MemoryLoader(std::initializer_list<ValueType> args) : Base(args)
	{
	}

	bool operator()(const std::string_view fileName, auto buffer) const noexcept
	{
		const auto it = this->find(std::string{fileName});
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