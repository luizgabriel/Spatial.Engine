#include <gtest/gtest.h>
#include <spatial/render/Light.h>
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

	auto actor = stage.createActor("Test Camera Actor").asCamera().withPerspectiveProjection().build();
	stage.setCamera(actor);

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

	auto actor = stage.createActor("Test Camera Actor").asCamera().withOrthographicProjection().build();
	stage.setCamera(actor);

	ASSERT_TRUE(actor == stage.getMainCamera());
	ASSERT_TRUE(actor.hasComponent<Camera>());

	auto& camera = actor.getComponent<Camera>();
	ASSERT_TRUE(camera.isValid());
	ASSERT_TRUE(camera.isOrthographic());
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