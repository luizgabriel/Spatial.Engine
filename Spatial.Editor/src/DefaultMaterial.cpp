#include "DefaultMaterial.h"

namespace spatial::editor
{

void DefaultMaterial::setParameters(filament::MaterialInstance& instance) const
{
	instance.setParameter("baseColor", baseColor);
	instance.setParameter("metallic", metallic);
	instance.setParameter("roughness", roughness);
	instance.setParameter("reflectance", reflectance);
}

} // namespace spatial::editor