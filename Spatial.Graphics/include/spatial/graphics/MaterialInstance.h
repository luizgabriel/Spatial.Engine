#pragma once

#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <memory>
#include <spatial/graphics/Resources.h>
#include <unordered_map>

namespace spatial::graphics
{

class MaterialInstance
{
  public:
	using MaterialInstanceResource = EngineResource<filament::MaterialInstance>;

	MaterialInstance(filament::Engine& engine, SharedMaterial material, filament::MaterialInstance* materialInstance);

	~MaterialInstance();

	MaterialInstance(MaterialInstance&& other) noexcept;

	MaterialInstance& operator=(MaterialInstance&& other) noexcept;

	MaterialInstance(const MaterialInstance& other) = delete;
	MaterialInstance& operator=(const MaterialInstance& other) = delete;

	const SharedMaterial& getMaterial() const
	{
		return mMaterial;
	}

	const filament::MaterialInstance* getInstance() const noexcept
	{
		return mInstance.get();
	}

	void setScissor(uint32_t left, uint32_t bottom, uint32_t width, uint32_t height);
	void setParameter(std::string_view parameter, bool value);
	void setParameter(std::string_view parameter, float value);
	void setParameter(std::string_view parameter, math::vec2 value);
	void setParameter(std::string_view parameter, math::vec3 value);
	void setParameter(std::string_view parameter, math::vec4 value);
	void setParameter(std::string_view parameter, filament::Texture* texture, filament::TextureSampler sampler);

  private:
	filament::Engine& mEngine;
	SharedMaterial mMaterial;
	MaterialInstanceResource mInstance;

	filament::MaterialInstance* get() noexcept
	{
		return mInstance.get();
	}
};

using SharedMaterialInstance = std::shared_ptr<graphics::MaterialInstance>;

SharedMaterialInstance toShared(MaterialInstance&& materialInstance);

MaterialInstance createMaterialInstance(filament::Engine& engine, const SharedMaterial& material,
										std::string_view = {}) noexcept;

} // namespace spatial::graphics