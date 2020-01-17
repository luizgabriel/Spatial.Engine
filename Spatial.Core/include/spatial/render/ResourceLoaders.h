#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/render/MeshResource.h>

#include <filesystem>

namespace spatial::render
{

Material createMaterial(filament::Engine *engine, const std::filesystem::path &name);

MeshResource createMesh(filament::Engine *engine, filament::MaterialInstance* material, const std::filesystem::path &name);

} // namespace spatial::render