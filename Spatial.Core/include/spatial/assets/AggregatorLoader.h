#pragma once

#include <spatial/assets/ResourcesLoader.h>
#include <sstream>
#include <string>
#include <tuple>

namespace spatial
{

template <typename Action = assets::ResourcesLoader>
struct AggregatorLoader : public std::vector<Action>
{
	using Base = std::vector<Action>;
	using ValueType = typename Base::value_type;

	AggregatorLoader(std::initializer_list<ValueType> args) : Base(args)
	{
	}

	assets::Resource operator()(const std::string_view fileName) const noexcept
	{
		for (auto& disk : *this)
		{
			auto resource = disk(fileName);
			if (resource)
				return resource;
		}

		return std::nullopt;
	}
};

} // namespace spatial