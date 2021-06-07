#pragma once

#include <spatial/common/Math.h>
#include <filament/MaterialInstance.h>

namespace spatial::editor
{

struct DefaultMaterial
{
	math::float3 baseColor;
	float metallic{.8f};
	float roughness{0.8f};
	float reflectance{.1f};

	void setParameters(filament::MaterialInstance& instance) const;
};

}