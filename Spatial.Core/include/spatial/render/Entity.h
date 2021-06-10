#pragma once

#include <filament/Engine.h>
#include <utils/EntityManager.h>

namespace spatial::render
{

class Entity
{
  private:
	filament::Engine& mEngine;
	utils::Entity mEntity;

  public:
	explicit Entity(filament::Engine& engine);

	Entity(filament::Engine& engine, utils::Entity entity);

	~Entity();

	Entity(Entity&& other) noexcept;
	Entity& operator=(Entity&& other) noexcept;

	Entity(const Entity& other) = delete;
	Entity& operator=(const Entity& w) = delete;

	[[nodiscard]] utils::Entity release() noexcept;

	void reset();

	bool isValid() const noexcept;

	utils::Entity get() const noexcept
	{
		return mEntity;
	}
};

} // namespace spatial::render