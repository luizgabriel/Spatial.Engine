#include <fstream>
#include <gtest/gtest.h>
#include <spatial/render/InstanceBuilder.h>
#include <spatial/render/Light.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/Stage.h>

using namespace spatial;

TEST(Stage, Create)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};
	const auto position = math::float3{1.0f, 2.0f, 3.0f};
	const auto scale = math::float3{10.0f, 20.0f, 30.0f};
	const auto rotation = math::float3{30.0f, 45.0f, 90.0f};

	auto instance = createInstance(stage).withPosition(position).withScale(scale).withRotation(rotation).get();
	ASSERT_TRUE(instance.isValid());
	ASSERT_TRUE(instance.has<Entity>());
	ASSERT_TRUE(instance.get<Entity>().isValid());

	ASSERT_NEAR(instance.get<Transform>().getPosition().x, position.x, math::epsilon);
	ASSERT_NEAR(instance.get<Transform>().getPosition().y, position.y, math::epsilon);
	ASSERT_NEAR(instance.get<Transform>().getPosition().z, position.z, math::epsilon);

	ASSERT_NEAR(instance.get<Transform>().getScale().x, scale.x, math::epsilon);
	ASSERT_NEAR(instance.get<Transform>().getScale().y, scale.y, math::epsilon);
	ASSERT_NEAR(instance.get<Transform>().getScale().z, scale.z, math::epsilon);
}

TEST(Stage, CreatePerspectiveCamera)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	auto instance =
		createInstance(stage, "Test Camera").asCamera().withPerspectiveProjection(45.0, 19 / 6.0, .1, 100.0).get();
	stage.setMainCamera(instance);

	ASSERT_TRUE(instance == stage.getMainCamera());
	ASSERT_TRUE(instance.has<Camera>());

	auto& camera = instance.get<Camera>();
	ASSERT_TRUE(camera.isValid());
	ASSERT_TRUE(camera.isPerspective());
}

TEST(Stage, CreateOrthographicCamera)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	auto instance =
		createInstance(stage, "Test Camera").asCamera().withOrthographicProjection(19 / 6, .1, 1000.0).get();
	stage.setMainCamera(instance);

	ASSERT_TRUE(instance == stage.getMainCamera());
	ASSERT_TRUE(instance.has<Camera>());

	auto& camera = instance.get<Camera>();
	ASSERT_TRUE(camera.isValid());
	ASSERT_TRUE(camera.isOrthographic());
}

TEST(Stage, CreateCustomCamera)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	auto instance =
		createInstance(stage, "Test Camera").asCamera().withCustomProjection(math::mat4{}, .1, 10000.0).get();

	stage.setMainCamera(instance);

	ASSERT_TRUE(instance == stage.getMainCamera());
	ASSERT_TRUE(instance.has<Camera>());

	auto& camera = instance.get<Camera>();
	ASSERT_TRUE(camera.isValid());
	ASSERT_TRUE(camera.isCustomProjection());
}

TEST(Stage, CreatePointLight)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	auto instance = createInstance(stage, "Test Point Light").asLight().withIntensity(10000.0f).get();

	ASSERT_TRUE(instance.has<Light>());

	auto& light = instance.get<Light>();
	ASSERT_TRUE(light.isValid());
	ASSERT_TRUE(light.isPointLight());
	ASSERT_FALSE(light.isSpotLight());
	ASSERT_FALSE(light.isDirectional());
	// ASSERT_EQ(light.getIntensity(), 10000.0f);
}

TEST(Stage, CreateDirectionalLight)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	auto instance = createInstance(stage, "Test Directional Light")
						.asLight(Light::Type::DIRECTIONAL)
						.withDirection({.0f, -1.0f, .0f})
						.withIntensity(10000.0f)
						.get();

	ASSERT_TRUE(instance.has<Light>());

	auto& light = instance.get<Light>();
	ASSERT_TRUE(light.isValid());
	ASSERT_TRUE(light.isDirectional());
	ASSERT_FALSE(light.isPointLight());
	ASSERT_FALSE(light.isSpotLight());
	ASSERT_EQ(light.getIntensity(), 10000.0f);
	ASSERT_EQ(light.getDirection(), math::float3(.0f, -1.0f, .0f));
}

TEST(Stage, CreateRenderable)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	auto planeFilameshStream = std::ifstream{"assets/plane.filamesh"};
	ASSERT_TRUE(planeFilameshStream);

	auto planeFilameshData = std::string{std::istreambuf_iterator{planeFilameshStream}, {}};
	ASSERT_GT(planeFilameshData.size(), 0);

	auto mesh = createFilamesh(renderingSystem.getEngine(), planeFilameshData);

	for (int i = 0; i < 10; i++)
		createInstance(stage).asMesh(mesh);

	auto view = stage.getInstances<const Renderable>();
	ASSERT_EQ(view.size(), 10);

	view.each([&](const Renderable& renderable) {
		ASSERT_EQ(renderable.getPrimitiveCount(), 1);
	});
}