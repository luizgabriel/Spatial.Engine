#pragma once

#include <spatial/ecs/Mesh.h>
#include <spatial/resources/FilameshFile.h>

namespace spatial
{

ecs::VertexData createVertexData(const FilameshFile& filamesh);

ecs::IndexData createIndexData(const FilameshFile& filamesh);

} // namespace spatial