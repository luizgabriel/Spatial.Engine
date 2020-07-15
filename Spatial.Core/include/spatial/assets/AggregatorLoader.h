#pragma once

#include <spatial/assets/AssetReadAction.h>
#include <sstream>
#include <string>
#include <tuple>

namespace spatial
{

template <typename BufferType = BasicBufferType>
struct AggregatorLoader : public std::vector<AssetReadAction<BufferType>>
{
	using Base = std::vector<AssetReadAction<BufferType>>;
	using ValueType = typename Base::value_type;

	AggregatorLoader(std::initializer_list<ValueType> args) : Base(args)
	{
	}

	bool operator()(const std::string_view fileName, BufferType buffer) const noexcept
	{
		for (auto& disk : *this)
			if (disk(fileName, buffer))
				return true;

		return false;
	}
};

} // namespace spatial