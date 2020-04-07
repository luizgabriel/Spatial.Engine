#include "Sandbox.h"

#include <spatial/render/CommonResources.h>
#include <spatial/render/ResourceLoaders.h>

#include <filament/RenderableManager.h>
#include <filament/LightManager.h>
#include <filament/TextureSampler.h>

using namespace spatial::render;
namespace fl = filament;

namespace spatial::sandbox
{

Sandbox::Sandbox(RenderingSystem& rendering)
	: m_engine{rendering.getEngine()},
	  m_camera{rendering.getMainCamera()},
	  m_view{rendering.getMainView()},

	  m_scene{createScene(m_engine)},
	  m_material{createMaterial(m_engine, "materials/default")},
	  m_instance{m_engine, m_material->createInstance()},
	  m_texture{createTexture(m_engine, "textures/debug_cube.png")},
	  m_light{createEntity(m_engine)},
	  m_sphereMesh{createMesh(m_engine, m_instance.get(), "models/debug_cube")},
	  m_ibl{createIBLFromKtx(m_engine, "textures/pillars_2k")}
{
}

void Sandbox::onStart()
{
	m_view->setScene(m_scene.get());

	const auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
	m_instance->setParameter("albedo", m_texture.get(), sampler);
	// m_instance->setParameter("baseColor", fl::RgbType::sRGB, {0.8, 0.0, 0.0});

	m_scene->setSkybox(m_ibl.getSkybox());
	m_scene->setIndirectLight(m_ibl.getLight());
	m_scene->addEntity(m_light.get());
	m_scene->addEntity(m_sphereMesh.get());

	auto& rcm = m_engine->getRenderableManager();
	const auto ri = rcm.getInstance(m_sphereMesh.get());
	rcm.setCastShadows(ri, false);
}

void Sandbox::onUpdateFrame(float delta)
{
	m_interface.render();

	auto& cameraData = m_interface.cameraData;
	auto& viewData = m_interface.viewData;
	auto& materialData = m_interface.materialData;

	m_camera->lookAt(cameraData.eye, cameraData.center, {0, 1.0f, .0f});
	m_view->setClearColor(viewData.clearColor);

	m_instance->setParameter("metallic", materialData.metallic);
	m_instance->setParameter("roughness", materialData.roughness);
	m_instance->setParameter("clearCoat", materialData.clearCoat);
	m_instance->setParameter("clearCoatRoughness", materialData.clearCoatRoughness);

	fl::LightManager::Builder(fl::LightManager::Type::SUN)
		.color(fl::Color::toLinear<fl::ACCURATE>({0.98f, 0.92f, 0.89f}))
		.intensity(110000.0f)
		.direction(cameraData.center - cameraData.eye)
		.castShadows(true)
		.sunAngularRadius(1.9f)
		.sunHaloSize(10.0f)
		.sunHaloFalloff(80.0f)
		.build(*m_engine, m_light.get());
}

} // namespace spatial::sandbox