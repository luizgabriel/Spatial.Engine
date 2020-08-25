#pragma once

#include <spatial/render/Entity.h>

namespace spatial
{

class Camera
{
	filament::Engine& mEngine;
	utils::Entity mEntity;
	filament::Camera* mResource;

  public:
	explicit Camera(filament::Engine& engine);
	Camera(filament::Engine& engine, utils::Entity entity);

	~Camera();

	Camera(Camera&& other) noexcept;
	Camera& operator=(Camera&& other) noexcept;

	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

	[[nodiscard]] const auto& ref() const
	{
		return *mResource;
	}

	[[nodiscard]] auto& ref()
	{
		return *mResource;
	}

	[[nodiscard]] auto* get()
	{
		return mResource;
	}

	[[nodiscard]] const auto* get() const
	{
		return mResource;
	}

	[[nodiscard]] auto* operator->()
	{
		return mResource;
	}

	[[nodiscard]] auto entity()
	{
		return mEntity;
	}
};

} // namespace spatial