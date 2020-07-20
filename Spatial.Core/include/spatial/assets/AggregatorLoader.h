#pragma once

#include <spatial/assets/ResourcesLoder.h>
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

	bool operator()(const std::string_view fileName, auto buffer) const noexcept
	{
		for (auto& disk : *this)
			if (disk(fileName, buffer))
				return true;

		return false;
	}
};

} // namespace spatial