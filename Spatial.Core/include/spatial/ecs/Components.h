#pragma once

#include <string>
#include <variant>
#include <entt/entity/entity.hpp>
#include <utils/Entity.h>
#include <spatial/common/Math.h>

namespace spatial::ecs
{

struct Transform
{
	math::float3 position{.0f};
	math::float3 scale{1.0f};
	math::float3 rotation{0.0f};
};

struct SceneEntity
{
	utils::Entity entity;
};

struct PointLight
{
	filament::math::float3 color{1.0f};
	float intensity{30000.0f};
	bool castShadows{true};
};

struct DirectionalLight
{
	math::float3 color{1.0f};
	float intensity{30000.0f};
	math::float3 direction{.0f, -1.0f, .0f};
	bool isSun{false};
	bool castShadows{true};
};

struct SpotLight
{
	filament::math::float3 color{1.0f};
	float intensity{30000.0f};
	filament::math::float3 direction{.0f, -1.0f, .0f};
	float innerCone{math::pi<float>/4};
	float outerCone{math::pi<float>/4};
	bool focused{false};
	bool castShadows{true};
};

struct Renderable
{
	std::uint32_t meshId;
	std::size_t materialId;
	std::size_t subMeshId{0};

	bool castShadows{false};
	bool receiveShadows{false};
};

struct Name
{
	std::string value;
};

struct OrtographicCamera
{
	float near{0.1f};
	float far{1000000.0f};
	float aspectRatio{16.0f / 9.0f};
};

struct PerspectiveCamera
{
	float near{0.1f};
	float far{1000000.0f};
	float aspectRatio{16.0f / 9.0f};
	float fieldOfView{45.0f};
};

struct CustomCamera
{
	float near{0.1f};
	float far{1000000.0f};
	filament::math::mat4 projection;
};

struct CameraTarget
{
	math::float3 target{.0f};
};

} // namespace spatial::ecs