#include <spatial/render/InstanceBuilder.h>
#include <spatial/render/InstanceHandle.h>
#include <spatial/render/SceneNodeName.h>
#include <spatial/render/Stage.h>

namespace spatial
{

Stage::Stage(filament::Engine& engine)
	: mEngine{engine}, mScene{createScene(mEngine)}, mView{createView(mEngine)}, mRegistry{}, mCameraInstance{}
{
	mView->setScene(mScene.get());
	mView->setBlendMode(fl::View::BlendMode::OPAQUE);

	mRegistry.on_construct<Entity>().connect<&Stage::onCreateRenderable>(this);
	mRegistry.on_destroy<Entity>().connect<&Stage::onDestroyRenderable>(this);
}

void Stage::onCreateRenderable(Stage::Registry& registry, Instance instance)
{
	mScene->addEntity(getComponent<Entity>(instance).get());
}

void Stage::onDestroyRenderable(Stage::Registry& registry, Instance instance)
{
	mScene->remove(getComponent<Entity>(instance).get());
}

Instance Stage::createInstance()
{
	return getRegistry().create();
}

Instance Stage::createInstance(const std::string_view name)
{
	auto handle = handleOf(*this, createInstance());
	handle.add<SceneNodeName>(std::string{name});

	return handle;
}

void Stage::setMainCamera(Instance instance)
{
	mCameraInstance = instance;
	auto handle = handleOf(*this, instance);

	if (handle.isValid())
	{
		auto& component = handle.get<spatial::Camera>();
		mView->setCamera(component.getInstance());
	}
	else
	{
		mView->setCamera(nullptr);
	}
}

void Stage::render(filament::Renderer& renderer) const
{
	renderer.render(mView.get());
}

void Stage::onStart()
{
	getInstances<Transform>().each([](auto& transform) { transform.refresh(); });
}

} // namespace spatial