#pragma once

#include <spatial/render/ActorBuilder.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Resources.h>
#include <spatial/render/StageView.h>
#include <string>

namespace spatial
{

class ActorBuilder;

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

	size_t size() const noexcept
	{
		return mRegistry.size();
	}

	template <typename... Component, typename... Exclude>
	auto getActorsWith(ExcludeComponent<Exclude...> excludes = {})
	{
		return StageView<ExcludeComponent<Exclude...>, Component...>{&mRegistry, mRegistry.view<Component...>(std::move(excludes))};
	}

	template <typename... Component, typename... Exclude>
	auto getFirstActorWith(ExcludeComponent<Exclude...> excludes = {})
	{
		auto view = getActorsWith<Component...>(std::move(excludes));
		return *view.begin();
	}

  private:
	RenderingSystem& mRenderingSystem;
	Scene mScene;
	SharedView mView;
	bool mEnabled;

	entt::registry mRegistry;

	Actor mCameraActor;
};

} // namespace spatial
