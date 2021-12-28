#include "Materials.h"

namespace spatial::editor
{

void DefaultMaterial::apply(filament::MaterialInstance& instance) const
{
	instance.setParameter("baseColor", baseColor);
	instance.setParameter("metallic", metallic);
	instance.setParameter("roughness", roughness);
	instance.setParameter("reflectance", reflectance);
}

void GridMaterial::apply(filament::MaterialInstance& instance) const
{
	instance.setParameter("color", color);
	instance.setParameter("scale", scale);
	instance.setParameter("thickness", thickness);
}

} // namespace spatial::editor