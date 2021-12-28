#include "spatial/ecs/Relation.h"
#include "spatial/ecs/SceneView.h"
#include "spatial/render/TextureView.h"
#include "spatial/ui/components/SceneView.h"
#include <array>
#include <imgui.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Tags.h>
#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DirectionInput.h>
#include <spatial/ui/components/Search.h>
#include <spatial/ui/components/VectorInput.h>

namespace spatial::ui
{

void spacing(std::uint32_t times)
{
	for (std::uint32_t i = 0; i < times; i++)
	{
		ImGui::Spacing();
	}
}

int inputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		auto* str = reinterpret_cast<std::string*>(data->UserData);
		str->resize(static_cast<unsigned long>(data->BufTextLen));
		data->Buf = str->data();
	}

	return 1;
};

bool inputText(const std::string_view label, std::string& value)
{
	return ImGui::InputText(label.data(), value.data(), value.size() + 1, ImGuiInputTextFlags_CallbackResize,
							&inputTextCallback, &value);
}

bool inputPath(const std::string_view label, std::filesystem::path& path)
{
	auto value = path.string();
	if (ui::inputText(label, value))
	{
		path = std::filesystem::path{value};
		return true;
	}

	return false;
}

template <>
void componentInput<ecs::DirectionalLight>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::DirectionalLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);

	directionInput("Direction", light.direction);
}

template <>
void componentInput<ecs::PointLight>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::PointLight>(entity);

	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ImGui::InputFloat("Falloff", &light.falloff);
}

template <>
void componentInput<ecs::SpotLight>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SpotLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ImGui::InputFloat("Falloff", &light.falloff);
	ImGui::DragFloat("Inner Angle", &light.innerAngle, math::pi / 180.0f, 0, math::pi / 2.0f);
	ImGui::DragFloat("Outer Angle", &light.outerAngle, math::pi / 180.0f, light.innerAngle, math::pi / 2.0f);

	directionInput("Direction", light.direction);
}

template <>
void componentInput<ecs::SunLight>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SunLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ImGui::InputFloat("Halo Falloff", &light.haloFalloff);
	ImGui::InputFloat("Halo size", &light.haloSize);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
template <>
void componentInput<ecs::Mesh>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& mesh = registry.getComponent<ecs::Mesh>(entity);
	bool changed = false;

	changed |= ui::inputPath("Resource", mesh.meshResource.relativePath);

	spacing(3);

	ImGui::Separator();

	spacing(3);

	changed |= ImGui::Checkbox("Cast Shadows", &mesh.castShadows);
	changed |= ImGui::Checkbox("Receive Shadows", &mesh.receiveShadows);

	spacing(3);

	const size_t smallStep = 1, largeStep = 5;

	spacing(3);

	if (ImGui::TreeNodeEx("Geometry Configuration", ImGuiTreeNodeFlags_SpanFullWidth))
	{
		spacing(3);

		changed |=
			ImGui::InputScalar("Parts Count", ImGuiDataType_U64, &mesh.partsCount, &smallStep, &largeStep, "%lu");
		changed |=
			ImGui::InputScalar("Parts Offset", ImGuiDataType_U64, &mesh.partsOffset, &smallStep, &largeStep, "%lu");
		ImGui::TreePop();

		spacing(3);
	}

	if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);

		const auto* parent = registry.tryGetComponent<ecs::Parent>(entity);
		const auto childrenCount = parent != nullptr ? parent->childrenCount : 0;

		if (ImGui::Button("Add Slot"))
		{
			auto child =
				ecs::build(registry).withName(fmt::format("Primitive {}", childrenCount)).with(ecs::MeshMaterial{});
			ecs::Parent::addChild(registry, entity, child);
			mesh.partsCount = std::max(mesh.partsCount, childrenCount + 1);
			changed = true;
		}

		static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
											 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
											 | ImGuiTableFlags_NoBordersInBody;

		if (ImGui::BeginTable("MaterialsTable", 3, flags))
		{

			ImGui::TableSetupColumn("Primitive Index");
			ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Actions");
			ImGui::TableHeadersRow();

			if (parent)
			{
				auto childrenToDestroy = std::vector<ecs::Entity>{};

				ecs::Parent::forEachChild(registry, entity, [&](ecs::Entity child) {
					auto& meshMaterial = registry.getComponent<ecs::MeshMaterial>(child);

					ImGui::PushID((int)child);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
					changed |= ImGui::InputScalar("##PrimitiveIndex", ImGuiDataType_U32, &meshMaterial.primitiveIndex,
												  &smallStep, &largeStep, "%lu");

					ImGui::TableNextColumn();

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
					changed |= ui::Search::searchEntity<ecs::tags::IsMaterial>("##Material", registry,
																			   meshMaterial.materialEntity);

					ImGui::TableNextColumn();

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
					if (ImGui::Button("Remove"))
						childrenToDestroy.emplace_back(child);

					ImGui::PopID();
				});

				for (auto child : childrenToDestroy)
				{
					ecs::Child::remove(registry, child);
					registry.destroy(child);
					mesh.partsCount = std::max(0ul, mesh.partsCount - 1);
					changed = true;
				}
			}

			ImGui::EndTable();
		}

		spacing(3);

		ImGui::TreePop();
	}

	if (changed)
		registry.addComponent<ecs::tags::IsRenderableDirty>(entity);
}
#pragma clang diagnostic pop

template <>
void componentInput<ecs::MeshMaterial>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& meshMaterial = registry.getComponent<ecs::MeshMaterial>(entity);
	float smallStep = 1;
	float largeStep = 1;

	ImGui::InputScalar("Primitive Index", ImGuiDataType_U32, &meshMaterial.primitiveIndex, &smallStep, &largeStep,
					   "%lu");
	ui::Search::searchEntity<ecs::tags::IsMaterial>("Material", registry, meshMaterial.materialEntity);
}

template <>
void componentInput<ecs::Transform>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& transform = registry.getComponent<ecs::Transform>(entity);

	vec3Input("Position", transform.position);

	auto eulerAnglesRotation = transform.getEulerAnglesRotation();
	vec3Input("Rotation", eulerAnglesRotation);
	transform.rotation = math::deg2rad * eulerAnglesRotation;

	vec3Input("Scale", transform.scale);
}

template <>
void componentInput<ecs::PerspectiveCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::PerspectiveCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	double min = 15.0, max = 120.0;
	ImGui::DragScalar("Field Of View", ImGuiDataType_Double, &camera.fieldOfView, 1.0f, &min, &max, "%.1f");
	ImGui::InputDouble("Aspect Ratio", &camera.aspectRatio);
}

template <>
void componentInput<ecs::OrthographicCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::OrthographicCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	ImGui::InputDouble("Left", &camera.left);
	ImGui::InputDouble("Right", &camera.right);
	ImGui::InputDouble("Bottom", &camera.bottom);
	ImGui::InputDouble("Top", &camera.top);
}

template <>
void componentInput<ecs::CustomCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::CustomCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	ImGui::InputScalarN("m0", ImGuiDataType_Double, &camera.projectionMatrix[0], 4);
	ImGui::InputScalarN("m1", ImGuiDataType_Double, &camera.projectionMatrix[1], 4);
	ImGui::InputScalarN("m2", ImGuiDataType_Double, &camera.projectionMatrix[2], 4);
	ImGui::InputScalarN("m3", ImGuiDataType_Double, &camera.projectionMatrix[3], 4);
}

template <>
void componentInput<ecs::SceneView>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& sceneView = registry.getComponent<ecs::SceneView>(entity);

	ImGui::DragInt2("Size", &sceneView.size.x);

	Search::searchEntity<ecs::IndirectLight>("Indirect Light", registry, sceneView.indirectLight);

	Search::searchEntity<ecs::tags::IsCamera>("Camera", registry, sceneView.camera);
	if (registry.hasAnyComponent<ecs::tags::IsCamera>(sceneView.camera)
		&& ImGui::TreeNodeEx("Camera Properties", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);
		if (registry.hasAllComponents<ecs::PerspectiveCamera>(sceneView.camera))
			ui::componentInput<ecs::PerspectiveCamera>(registry, sceneView.camera);
		if (registry.hasAllComponents<ecs::OrthographicCamera>(sceneView.camera))
			ui::componentInput<ecs::OrthographicCamera>(registry, sceneView.camera);
		if (registry.hasAllComponents<ecs::CustomCamera>(sceneView.camera))
			ui::componentInput<ecs::CustomCamera>(registry, sceneView.camera);
		spacing(3);
		ImGui::TreePop();
	}

	if (registry.hasAllComponents<render::TextureView>(entity)
		&& ImGui::TreeNodeEx("Camera Preview", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		SceneView::image(registry, entity, {ImGui::GetContentRegionAvailWidth(), 100});
		ImGui::TreePop();
	}
}

void image(const filament::Texture& texture, math::float2 size, math::float4 uv)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
	ImGui::Image((ImTextureID)&texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
#pragma clang diagnostic pop
}

bool imageButton(const filament::Texture& texture, math::float2 size, math::float4 uv)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
	return ImGui::ImageButton((ImTextureID)&texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
#pragma clang diagnostic pop
}

} // namespace spatial::ui