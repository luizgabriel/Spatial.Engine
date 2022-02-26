#pragma once

#include <filament/Engine.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <memory>
#include <spatial/render/EngineResource.h>

namespace spatial::render
{

class MaterialInstance
{
  public:
	using SharedMaterial = SharedEngineResource<filament::Material>;
	using MaterialInstanceResource = EngineResource<filament::MaterialInstance>;

	MaterialInstance(filament::Engine& engine, SharedMaterial material, filament::MaterialInstance* materialInstance);

	MaterialInstance(MaterialInstance&& other) noexcept;

	MaterialInstance& operator=(MaterialInstance&& other) noexcept;

	MaterialInstance(const MaterialInstance& other) = delete;
	MaterialInstance& operator=(const MaterialInstance& other) = delete;

	const SharedMaterial& getMaterial() const
	{
		return mMaterial;
	}

	const filament::MaterialInstance* get() const noexcept
	{
		return mInstance.get();
	}

	filament::MaterialInstance* get() noexcept
	{
		return mInstance.get();
	}

	const filament::MaterialInstance* operator->() const noexcept
	{
		return mInstance.get();
	}

	filament::MaterialInstance* operator->() noexcept
	{
		return mInstance.get();
	}

	const filament::MaterialInstance& operator*() const
	{
		return *mInstance;
	}

	filament::MaterialInstance& operator*()
	{
		return *mInstance;
	}

  private:
	filament::Engine& mEngine;
	SharedMaterial mMaterial;
	MaterialInstanceResource mInstance;
};

std::shared_ptr<MaterialInstance> toShared(MaterialInstance&& materialInstance);

} // namespace spatial::render