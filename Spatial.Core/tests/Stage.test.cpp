#include <fmt/format.h>
#include <gtest/gtest.h>
#include <spatial/render/Light.h>
#include <spatial/render/Name.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Stage.h>

using namespace spatial;

TEST(Stage, CreateActor)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	auto actor = stage.createActor("Test Actor").build();
	ASSERT_TRUE(actor.isValid());
	ASSERT_TRUE(actor.hasComponent<Entity>());
	ASSERT_TRUE(actor.getComponent<Entity>().isValid());
}

TEST(Stage, CreatePerspectiveCameraActor)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	auto actor = stage.createActor("Test Camera Actor").asCamera().withPerspectiveProjection(45.0, 19/6.0, .1, 100.0).build();
	stage.setMainCamera(actor);

	ASSERT_TRUE(actor == stage.getMainCamera());
	ASSERT_TRUE(actor.hasComponent<Camera>());

	auto& camera = actor.getComponent<Camera>();
	ASSERT_TRUE(camera.isValid());
	ASSERT_TRUE(camera.isPerspective());
}

TEST(Stage, CreateOrthographicCameraActor)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	auto actor = stage.createActor("Test Camera Actor").asCamera().withOrthographicProjection(19/6, .1, 1000.0).build();
	stage.setMainCamera(actor);

	ASSERT_TRUE(actor == stage.getMainCamera());
	ASSERT_TRUE(actor.hasComponent<Camera>());

	auto& camera = actor.getComponent<Camera>();
	ASSERT_TRUE(camera.isValid());
	ASSERT_TRUE(camera.isOrthographic());
}

TEST(Stage, CreateCustomCameraActor)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	auto actor = stage.createActor("Test Camera Actor")
					 .asCamera()
					 .withCustomProjection(math::mat4{}, .1, 10000.0)
					 .build();

	stage.setMainCamera(actor);

	ASSERT_TRUE(actor == stage.getMainCamera());
	ASSERT_TRUE(actor.hasComponent<Camera>());

	auto& camera = actor.getComponent<Camera>();
	ASSERT_TRUE(camera.isValid());
	ASSERT_TRUE(camera.isCustomProjection());
}

TEST(Stage, CreatePointLightActor)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	auto actor =
		stage.createActor("Test Point Light Actor").withPosition({5.0f}).asLight().withIntensity(1000.0f).build();

	ASSERT_TRUE(actor.hasComponent<Light>());

	auto& light = actor.getComponent<Light>();
	ASSERT_TRUE(light.isValid());
	ASSERT_TRUE(light.isPointLight());
	ASSERT_FALSE(light.isSpotLight());
	ASSERT_FALSE(light.isDirectional());
}

TEST(Stage, CreateDirectionalLightActor)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	auto actor = stage.createActor("Test Directional Light Actor")
					 .withPosition({5.0f})
					 .asLight(Light::Type::DIRECTIONAL)
					 .withDirection({100.0f, -100.0f, 100.0f})
					 .withIntensity(1000.0f)
					 .build();

	ASSERT_TRUE(actor.hasComponent<Light>());

	auto& light = actor.getComponent<Light>();
	ASSERT_TRUE(light.isValid());
	ASSERT_TRUE(light.isDirectional());
	ASSERT_FALSE(light.isPointLight());
	ASSERT_FALSE(light.isSpotLight());
}

TEST(StageView, CountActors)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	for (int i = 0; i < 10; i++)
		stage.createActor(fmt::format("Test {}", i));

	ASSERT_EQ(10, stage.size());
}

TEST(StageView, LoopForEachActor)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	for (int i = 0; i < 10; i++)
		stage.createActor(fmt::format("Test {}", i));

	int j = 0;
	for (auto actor : stage.getActorsWith<Name>()) {
		ASSERT_TRUE(actor.hasComponent<Name>());
		j++;
	}

	ASSERT_EQ(10, j);
}

TEST(StageView, LoopForCustomComponent)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	for (int i = 0; i < 10; i++)
		stage.createActor(fmt::format("Test {}", i));

	struct AnotherComponent{ int i = 42; };
	for (int i = 0; i < 3; i++)
		stage.createActor(fmt::format("Test {}", i)).add<AnotherComponent>();

	int j = 0;
	for (auto actor : stage.getActorsWith<AnotherComponent>()) {
		ASSERT_TRUE(actor.hasComponent<AnotherComponent>());
		j++;
	}

	ASSERT_EQ(3, j);
}

TEST(StageView, LoopForComponentForeach)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem};

	for (int i = 0; i < 10; i++)
		stage.createActor(fmt::format("Test {}", i));

	stage.getActorsWith<Name>().forEach([](auto& name) {
		ASSERT_TRUE(name.getValue().size() > 0);
	});
}