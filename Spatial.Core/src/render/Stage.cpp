#include <spatial/render/Name.h>
#include <spatial/render/Stage.h>

namespace spatial
{

Stage::Stage(RenderingSystem& renderingSystem)
	: mRenderingSystem{renderingSystem},
	  mScene{createScene(mRenderingSystem.getEngine())},
	  mView{toShared(createView(mRenderingSystem.getEngine()))},
	  mEnabled{false},
	  mRegistry{},
	  mCameraActor{}
{
	mView->setScene(mScene.get());
	mView->setBlendMode(fl::View::BlendMode::OPAQUE);
}

Stage::~Stage()
{
	if (mEnabled)
		disable();
}

void Stage::enable()
{
	mEnabled = true;
	mRenderingSystem.pushBackView(mView);
}

void Stage::disable()
{
	mEnabled = false;
	mRenderingSystem.popView(mView);
}

ActorBuilder Stage::createActor(std::string name)
{
	return ActorBuilder{mRenderingSystem.getEngine(), mRegistry}
		.add<spatial::Name>(std::move(name));
}

void Stage::setMainCamera(const Actor& actor)
{
	mCameraActor = actor;

	if (mCameraActor.isValid())
	{
		auto& component = mCameraActor.getComponent<spatial::Camera>();
		mView->setCamera(component.getInstance());
	}
	else
	{
		mView->setCamera(nullptr);
	}
}

} // namespace spatial