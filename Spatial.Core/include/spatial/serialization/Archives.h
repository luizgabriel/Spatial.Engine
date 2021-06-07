#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

namespace spatial
{

using JsonOuputArchive = cereal::JSONOutputArchive;
using JsonInputArchive = cereal::JSONInputArchive;

using XMLInputArchive = cereal::XMLInputArchive;
using XMLOutputArchive = cereal::XMLOutputArchive;

using BinaryOutputArchive = cereal::BinaryOutputArchive;
using BinaryInputArchive = cereal::BinaryInputArchive;

} // namespace spatial