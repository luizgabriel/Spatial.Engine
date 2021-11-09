#pragma once

#include <cereal/cereal.hpp>
#include <filesystem>

namespace cereal
{

template <typename Archive>
void load(Archive& ar, std::filesystem::path& path)
{
	std::string value;
	ar(make_nvp("value", value));
	path = value;
}

template <typename Archive>
void save(Archive& ar, const std::filesystem::path& path)
{
	ar(make_nvp("value", path.string()));
}

} // namespace cereal