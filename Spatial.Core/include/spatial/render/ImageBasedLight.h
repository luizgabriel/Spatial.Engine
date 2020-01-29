#pragma once

#include <spatial/render/CommonResources.h>

#include <filament/Texture.h>
#include <math/vec3.h>

namespace spatial::render
{

class ImageBasedLight
{
private:
    IndirectLight m_light;
    Texture m_texture;

    Skybox m_skybox;
    Texture m_skyboxTexture;

public:
    ImageBasedLight(
        filament::Engine *engine,
        filament::IndirectLight *light,
        filament::Texture *texture,
        filament::Skybox *skybox,
        filament::Texture *skyboxTexture)
        : m_light{engine, light},
          m_texture{engine, texture},
          m_skybox{engine, skybox},
          m_skyboxTexture{engine, skyboxTexture}
    {
    }

    ImageBasedLight(
        IndirectLight&& light,
        Texture&& texture,
        Skybox&& skybox,
        Texture&& skyboxTexture)
        : m_light{std::move(light)},
          m_texture{std::move(texture)},
          m_skybox{std::move(skybox)},
          m_skyboxTexture{std::move(skyboxTexture)}
    {
    }

    const auto getSkybox() const
    {
        return m_skybox.get();
    }

    const auto getLight() const
    {
        return m_light.get();
    }
};

} // namespace render