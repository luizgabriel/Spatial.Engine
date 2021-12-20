#pragma once

#include <filament/MaterialInstance.h>
#include <spatial/common/Math.h>

namespace spatial::editor
{

struct DefaultMaterial
{
	math::float3 baseColor{1.0f};
	float metallic{.8f};
	float roughness{0.8f};
	float reflectance{.1f};

	void setParameters(filament::MaterialInstance& instance) const;
};

} // namespace spatial::editor