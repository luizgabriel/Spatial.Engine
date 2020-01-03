#pragma once

#include <spatial/render/Entity.h>
#include <spatial/render/Engine.h>
#include <spatial/render/CommonResources.h>

namespace spatial::render
{

class Sphere
{
public:
    Sphere(RenderEngine& engine, Material& material, bool culling);

    Sphere(Sphere &&rhs) noexcept;

    Sphere(Sphere const &) = delete;
    Sphere &operator=(Sphere const &) = delete;

    Entity getEntity()
    {
        return m_renderable;
    }

    const MaterialInstance& getMaterialInstance() const
    {
        return m_materialInstance;
    }

    MaterialInstance& getMaterialInstance()
    {
        return m_materialInstance;
    }

    Sphere &setPosition(filament::math::float3 const &position) noexcept;
    Sphere &setRadius(float radius) noexcept;

private:
    RenderEngine *m_engine;
    MaterialInstance m_materialInstance;
    Entity m_renderable;
};

} // namespace spatial::render