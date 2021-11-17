#pragma once

#include <filament/Color.h>
#include <filament/Engine.h>
#include <filament/LightManager.h>
#include <spatial/common/Math.h>

namespace spatial::render
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

	Light(Light&& other) noexcept;
	Light& operator=(Light&& other) noexcept;

	~Light();

	[[nodiscard]] bool isValid() const noexcept;

	[[nodiscard]] Type getType() const noexcept;

	[[nodiscard]] bool isDirectional() const noexcept;

	[[nodiscard]] bool isPointLight() const noexcept;

	[[nodiscard]] bool isSpotLight() const noexcept;

	void setDirection(const math::float3& direction) noexcept;

	[[nodiscard]] const math::float3& getDirection() const noexcept;

	void setColor(const filament::LinearColor& color) noexcept;

	[[nodiscard]] const math::float3& getColor() const noexcept;

	void setIntensity(float intensity) noexcept;

	void setIntensity(float watts, float efficiency) noexcept;

	void setIntensityCandela(float intensity) noexcept;

	[[nodiscard]] float getIntensity() const noexcept;

	void setFalloff(float radius) noexcept;

	[[nodiscard]] float getFalloff() const noexcept;

	void setSpotLightCone(float inner, float outer) noexcept;

	[[nodiscard]] float getSpotLightOuterCone() const noexcept;

	void setSunAngularRadius(float angularRadius) noexcept;

	[[nodiscard]] float getSunAngularRadius() const noexcept;

	void setSunHaloSize(float haloSize) noexcept;

	[[nodiscard]] float getSunHaloSize() const noexcept;

	void setSunHaloFalloff(float haloFalloff) noexcept;

	[[nodiscard]] float getSunHaloFalloff() const noexcept;

	[[nodiscard]] ShadowOptions const& getShadowOptions() const noexcept;

	void setShadowOptions(ShadowOptions const& options) noexcept;

	void setShadowCaster(bool shadowCaster) noexcept;

	[[nodiscard]] bool isShadowCaster() const noexcept;



  private:
	Manager& mManager;
	utils::Entity mEntity;

	Instance getInstance() const noexcept;

	void reset() noexcept;

	[[nodiscard]] utils::Entity release() noexcept;
};

} // namespace spatial::render