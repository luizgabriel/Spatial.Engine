#pragma once

#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Resources.h>
#include <spatial/render/ActorBuilder.h>
#include <string>

#include <entt/entity/registry.hpp>

namespace spatial
{

class Stage
{
  public:
	explicit Stage(RenderingSystem& renderingSystem);

	~Stage();

	void enable();

	void disable();

	ActorBuilder createActor(std::string name);

	auto& getView() noexcept
	{
		return mView;
	}

	auto& getScene() noexcept
	{
		return mScene.ref();
	}

	[[nodiscard]] bool isValid(entt::entity entity) const
	{
		return mRegistry.valid(entity);
	}

	void setMainCamera(const Actor& actor);

	const Actor& getMainCamera() const noexcept
	{
		return mCameraActor;
	}

	Actor& getMainCamera() noexcept
	{
		return mCameraActor;
	}

	const auto& getRegistry() const noexcept
	{
		return mRegistry;
	}

	auto& getRegistry() noexcept
	{
		return mRegistry;
	}

	Actor getActor(entt::entity entity)
	{
		return {&mRegistry, entity};
	}

	ConstActor getActor(entt::entity entity) const
	{
		return {&mRegistry, entity};
	}

  private:
	RenderingSystem& mRenderingSystem;
	Scene mScene;
	SharedView mView;
	bool mEnabled;

	entt::registry mRegistry;

	Actor mCameraActor;
};

} // namespace spatial::ecs
