#pragma once

#include <spatial/common/Math.h>
#include <filament/Color.h>
#include <filament/Engine.h>
#include <filament/LightManager.h>

namespace spatial
{

class Light
{
  public:
	using Manager = filament::LightManager;
	using Instance = Manager::Instance;
	using Type = Manager::Type;
	using ShadowOptions = Manager::ShadowOptions;
	using Builder = Manager::Builder;

	Light(filament::Engine& engine, utils::Entity entity, Type type = Type::POINT);

	Light(const Light& other) = delete;
	Light& operator=(const Light& other) = delete;

	Light(Light&& other);
	Light& operator=(Light&& other);

	~Light();

	bool isValid() const noexcept;

	Type getType() const noexcept;

	bool isDirectional() const noexcept;

	bool isPointLight() const noexcept;

	bool isSpotLight() const noexcept;

	void setDirection(const math::float3& direction) noexcept;

	const math::float3& getDirection() const noexcept;

	void setColor(const filament::LinearColor& color) noexcept;

	const math::float3& getColor() const noexcept;

	void setIntensity(float intensity) noexcept;

	void setIntensity(float watts, float efficiency) noexcept;

	void setIntensityCandela(float intensity) noexcept;

	float getIntensity() const noexcept;

	void setFalloff(float radius) noexcept;

	float getFalloff() const noexcept;

	void setSpotLightCone(float inner, float outer) noexcept;

	float getSpotLightOuterCone() const noexcept;

	void setSunAngularRadius(float angularRadius) noexcept;

	float getSunAngularRadius() const noexcept;

	void setSunHaloSize(float haloSize) noexcept;

	float getSunHaloSize() const noexcept;

	void setSunHaloFalloff(float haloFalloff) noexcept;

	float getSunHaloFalloff() const noexcept;

	ShadowOptions const& getShadowOptions() const noexcept;

	void setShadowOptions(ShadowOptions const& options) noexcept;

	void setShadowCaster(bool shadowCaster) noexcept;

	bool isShadowCaster() const noexcept;

  private:
	Manager& mManager;
	utils::Entity mEntity;

	Instance getInstance() const noexcept;
};

} // namespace spatial