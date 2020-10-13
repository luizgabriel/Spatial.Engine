#pragma once

#include "ResourcesLoader.h"
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <initializer_list>

namespace spatial::assets
{

class MemoryLoader : public ResourcesLoader
{
  private:
	std::unordered_map<std::string, std::pair<const char*, std::size_t>> mData;

  public:
	using ValueType = typename decltype(mData)::value_type;

	MemoryLoader();

	explicit MemoryLoader(std::size_t size);

	MemoryLoader(std::initializer_list<ValueType> values);
	MemoryLoader(const MemoryLoader& other) = delete;
	MemoryLoader(MemoryLoader&& other) = default;

	MemoryLoader&& add(std::string resourceName, std::pair<const char*, std::size_t> resourceData);

	virtual ResourceData load(const std::string_view fileName) const override;
};

} // namespace spatial