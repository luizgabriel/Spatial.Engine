#include <entt/entity/snapshot.hpp>
#include <spatial/render/Stage.h>
#include <cereal/archives/json.hpp>

namespace spatial
{

using JsonOuputArchive = cereal::JSONOutputArchive;
using JsonInputArchive = cereal::JSONInputArchive;

using BinaryOutputArchive = cereal::BinaryOutputArchive;
using BinaryInputArchive = cereal::BinaryInputArchive;

} // namespace spatial