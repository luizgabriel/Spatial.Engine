#include <spatial/graphics/Light.h>
#include <spatial/graphics/MathConversionUtils.h>

namespace spatial::graphics
{

Light::Light(filament::Engine& engine, utils::Entity entity, Light::Type type)
	: mManager{engine.getLightManager()}, mEntity{entity}
{
	if (Builder(type).castShadows(true).build(engine, entity) != Builder::Result::Success)
	{
		throw std::runtime_error("Could not create light");
	}
}

Light::Light(Light&& other) noexcept : mManager{other.mManager}, mEntity{other.release()}
{
}

Light& Light::operator=(Light&& other) noexcept
{
	reset();
	mEntity = other.release();
	return *this;
}

Light::~Light()
{
	reset();
}

bool Light::isValid() const noexcept
{
	return getInstance().isValid();
}

Light::Type Light::getType() const noexcept
{
	return mManager.getType(getInstance());
}

bool Light::isDirectional() const noexcept
{
	return mManager.isDirectional(getInstance());
}

bool Light::isPointLight() const noexcept
{
	return mManager.isPointLight(getInstance());
}

bool Light::isSpotLight() const noexcept
{
	return mManager.isSpotLight(getInstance());
}

void Light::setDirection(const math::vec3& direction) noexcept
{
	mManager.setDirection(getInstance(), toFilament(direction));
}

math::vec3 Light::getDirection() const noexcept
{
	return fromFilament(mManager.getDirection(getInstance()));
}

void Light::setColor(const filament::LinearColor& color) noexcept
{
	mManager.setColor(getInstance(), filament::Color::toLinear<filament::ColorConversion::ACCURATE>(color));
}

math::vec3 Light::getColor() const noexcept
{
	return fromFilament(mManager.getColor(getInstance()));
}

void Light::setIntensity(float intensity) noexcept
{
	mManager.setIntensity(getInstance(), intensity);
}

void Light::setIntensity(float watts, float efficiency) noexcept
{
	mManager.setIntensity(getInstance(), watts, efficiency);
}

void Light::setIntensityCandela(float intensity) noexcept
{
	mManager.setIntensityCandela(getInstance(), intensity);
}

float Light::getIntensity() const noexcept
{
	return mManager.getIntensity(getInstance());
}

void Light::setFalloff(float radius) noexcept
{
	mManager.setFalloff(getInstance(), radius);
}

float Light::getFalloff() const noexcept
{
	return mManager.getFalloff(getInstance());
}

void Light::setSpotLightCone(float inner, float outer) noexcept
{
	mManager.setSpotLightCone(getInstance(), inner, outer);
}

float Light::getSpotLightOuterCone() const noexcept
{
	return mManager.getSpotLightOuterCone(getInstance());
}

void Light::setSunAngularRadius(float angularRadius) noexcept
{
	mManager.setSunAngularRadius(getInstance(), angularRadius);
}

float Light::getSunAngularRadius() const noexcept
{
	return mManager.getSunAngularRadius(getInstance());
}

void Light::setSunHaloSize(float haloSize) noexcept
{
	mManager.setSunHaloSize(getInstance(), haloSize);
}

float Light::getSunHaloSize() const noexcept
{
	return mManager.getSunHaloSize(getInstance());
}

void Light::setSunHaloFalloff(float haloFalloff) noexcept
{
	mManager.setSunHaloFalloff(getInstance(), haloFalloff);
}

float Light::getSunHaloFalloff() const noexcept
{
	return mManager.getSunHaloFalloff(getInstance());
}

const Light::ShadowOptions& Light::getShadowOptions() const noexcept
{
	return mManager.getShadowOptions(getInstance());
}

void Light::setShadowOptions(const Light::ShadowOptions& options) noexcept
{
	mManager.setShadowOptions(getInstance(), options);
}

void Light::setShadowCaster(bool shadowCaster) noexcept
{
	mManager.setShadowCaster(getInstance(), shadowCaster);
}

bool Light::isShadowCaster() const noexcept
{
	return mManager.isShadowCaster(getInstance());
}

Light::Instance Light::getInstance() const noexcept
{
	return mManager.getInstance(mEntity);
}

utils::Entity Light::release() noexcept
{
	return std::exchange(mEntity, {});
}

void Light::reset() noexcept
{
	if (isValid())
		mManager.destroy(mEntity);

	mEntity = utils::Entity{};
}

} // namespace spatial::graphics