#pragma once

#include <filament/Texture.h>
#include <spatial/resources/ResourceFinder.h>

namespace spatial::render
{

template <typename T>
using is_image_texture_finder = is_resource_finder<T, ResourceType::ImageTexture, const filament::Texture*>;

template <typename T>
constexpr auto is_image_texture_finder_v = is_image_texture_finder<T>::value;

template <typename T>
using is_cubemap_texture_finder = is_resource_finder<T, ResourceType::CubeMapTexture, const filament::Texture*>;

template <typename T>
constexpr auto is_cubemap_texture_finder_v = is_cubemap_texture_finder<T>::value;

using ImageTextureResourceFinder = ResourceFinder<ResourceType::ImageTexture, const filament::Texture*>;
using CubeMapTextureResourceFinder = ResourceFinder<ResourceType::CubeMapTexture, const filament::Texture*>;

} // namespace spatial::render