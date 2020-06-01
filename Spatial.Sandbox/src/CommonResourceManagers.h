#pragma once

#include <spatial/render/Resources.h>
#include <spatial/render/ResourceLoaders.h>
#include "ResourceManager.h"

namespace spatial
{

using TextureManager = ResourceManager<Texture, std::string, &createTexture>;

using MaterialManager = ResourceManager<Material, std::string, &createMaterial>;

using MaterialInstanceManager = ResourceManager<MaterialInstance, std::uint32_t, &createMaterialInstance>;

using MeshManager = ResourceManager<Mesh, std::string, &createMesh>;

}