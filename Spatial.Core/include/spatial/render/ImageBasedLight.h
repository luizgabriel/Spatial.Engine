#pragma once

#include <spatial/render/Resources.h>

#include <filament/Texture.h>

namespace spatial
{

class ImageBasedLight
{
private:
	IndirectLight m_light;
	Texture m_texture;

	Skybox m_skybox;
	Texture m_skyboxTexture;

public:
	ImageBasedLight(filament::Engine* engine, filament::IndirectLight* light, filament::Texture* texture,
					filament::Skybox* skybox, filament::Texture* skyboxTexture)
		: m_light{createResource(engine, light)},
		  m_texture{createResource(engine, texture)},
		  m_skybox{createResource(engine, skybox)},
		  m_skyboxTexture{createResource(engine, skyboxTexture)}
	{
	}

	ImageBasedLight(IndirectLight&& light, Texture&& texture, Skybox&& skybox, Texture&& skyboxTexture)
		: m_light{std::move(light)},
		  m_texture{std::move(texture)},
		  m_skybox{std::move(skybox)},
		  m_skyboxTexture{std::move(skyboxTexture)}
	{
	}

	ImageBasedLight(const ImageBasedLight& other) = delete;
	ImageBasedLight& operator=(const ImageBasedLight& w) = delete;

	[[nodiscard]] auto getSkybox() const { return m_skybox.get(); }

	[[nodiscard]] auto getLight() const { return m_light.get(); }
};

} // namespace spatial