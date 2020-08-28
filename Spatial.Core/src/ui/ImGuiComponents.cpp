#include <array>
#include <imgui.h>
#include <imgui_internal.h>
#include <spatial/ui/ImGuiComponents.h>

namespace spatial::ui
{

void transformInput(ecs::Transform& transform, const std::string_view format)
{
	for (char c : format)
	{
		switch (c)
		{
		case 'p':
			ImGui::DragFloat3("Position", &transform.position[0]);
			break;
		case 'r':
			ImGui::DragFloat3("Rotation", &transform.rotation[0], math::pi<double> / 20.0, .0f, .0f);
			break;
		case 'y':
			ImGui::DragFloat2("Rotation", &transform.rotation[0], math::pi<double> / 20.0, .0f, .0f);
			break;
		case 's':
			ImGui::DragFloat3("Scale", &transform.scale[0]);
			break;
		}
	}
}

void cameraInput(ecs::Camera& camera)
{
	ImGui::InputFloat3("Target", &camera.target[0]);

	auto isPerspective = std::holds_alternative<ecs::Camera::Perspective>(camera.projection);
	auto isOrthographic = std::holds_alternative<ecs::Camera::Ortographic>(camera.projection);
	auto isCustom = std::holds_alternative<ecs::Camera::Custom>(camera.projection);
	auto options = std::array{"Perspective", "Orthographic", "Custom"};
	int currentItem = -1;

	if (isPerspective)
		currentItem = 0;
	else if (isOrthographic)
		currentItem = 1;
	else if (isCustom)
		currentItem = 2;

	if (ImGui::Combo("Projection", &currentItem, &options[0], options.size()))
	{
		if (currentItem == 0)
			camera.projection = ecs::Camera::Perspective{};
		else if (currentItem == 1)
			camera.projection = ecs::Camera::Ortographic{};
		else if (currentItem == 2)
			camera.projection = ecs::Camera::Custom{};
	}

	if (currentItem == 0)
	{
		auto& proj = std::get<ecs::Camera::Perspective>(camera.projection);
		ImGui::SliderFloat("Field of View", &proj.fieldOfView, 15.0f, 120.0f, "%.0f");
		ImGui::InputFloat("Aspect Ratio", &proj.aspectRatio);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	}
	else if (currentItem == 1)
	{
		auto& proj = std::get<ecs::Camera::Ortographic>(camera.projection);
		ImGui::InputFloat("Aspect Ratio", &proj.aspectRatio);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	}
	else if (currentItem == 2)
	{
		auto& proj = std::get<ecs::Camera::Custom>(camera.projection);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	}
}

bool sceneHierarchy(const entt::registry& registry, entt::entity& selectedEntity)
{
	const auto view = registry.view<const ecs::Name>();
	bool hasSelectedEntity = false;

	for (entt::entity entity : view)
	{
		const auto& name = view.get<const ecs::Name>(entity);
		ImGuiTreeNodeFlags flags = (selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(entity), flags, "%s", name.value.c_str());
		if (ImGui::IsItemClicked())
		{
			selectedEntity = entity;
		}

		if (opened)
			ImGui::TreePop();
	}

	return hasSelectedEntity;
}

int inputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		auto* str = reinterpret_cast<std::string*>(data->UserData);
		str->resize(data->BufTextLen);
		data->Buf = str->data();
	}

	return 1;
};

void inputText(const std::string_view label, std::string& value)
{
	ImGui::InputText(label.data(), value.data(), value.size() + 1, ImGuiInputTextFlags_CallbackResize,
					 &inputTextCallback, &value);
}

float quatD(float w, float h)
{
	return std::min(std::abs(w), std::abs(h)) - 4.0f;
}

float quatPX(float x, float w, float h)
{
	return (x * 0.5f * quatD(w, h) + w * 0.5f + 0.5f);
}

float quatPY(float y, float w, float h)
{
	return (-y * 0.5f * quatD(w, h) + h * 0.5f - 0.5f);
}

float quatIX(int x, float w, float h)
{
	return (2.0f * x - w - 1.0f) / quatD(w, h);
}

float quatIY(int y, float w, float h)
{
	return (-2.0f * y + h - 1.0f) / quatD(w, h);
}

static ImVector<math::float3> s_ArrowTri[4];
static ImVector<ImVec2> s_ArrowTriProj[4];
static ImVector<math::float3> s_ArrowNorm[4];
static ImVector<ImU32> s_ArrowColLight[4];

void initArrowData()
{
	constexpr int SUBDIV = 15;
	constexpr float CYL_RADIUS = 0.08f;
	constexpr float CONE_RADIUS = 0.16f;
	constexpr float CONE_LENGTH = 0.25f;
	constexpr float ARROW_BGN = -1.1f;
	constexpr float ARROW_END = 1.15f;
	constexpr int ARROW_CONE = 0;
	constexpr int ARROW_CONE_CAP = 1;
	constexpr int ARROW_CYL = 2;
	constexpr int ARROW_CAP = 3;

	for (int i = 0; i < 4; ++i)
	{
		s_ArrowTri[i].clear();
		s_ArrowNorm[i].clear();
	}

	float x0, x1, y0, y1, z0, z1, a0, a1, nx, nn;
	for (int i = 0; i < SUBDIV; ++i)
	{
		a0 = 2.0f * math::pi<float> * (float(i)) / SUBDIV;
		a1 = 2.0f * math::pi<float> * (float(i + 1)) / SUBDIV;
		x0 = ARROW_BGN;
		x1 = ARROW_END - CONE_LENGTH;
		y0 = cosf(a0);
		z0 = sinf(a0);
		y1 = cosf(a1);
		z1 = sinf(a1);
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x1, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x0, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x1, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x1, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y1, z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y1, z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y1, z1));
		s_ArrowTri[ARROW_CAP].push_back(math::float3(x0, 0, 0));
		s_ArrowTri[ARROW_CAP].push_back(math::float3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CAP].push_back(math::float3(x0, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowNorm[ARROW_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CAP].push_back(math::float3(-1, 0, 0));
		x0 = ARROW_END - CONE_LENGTH;
		x1 = ARROW_END;
		nx = CONE_RADIUS / (x1 - x0);
		nn = 1.0f / sqrtf(nx * nx + 1);
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x1, 0, 0));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x0, CONE_RADIUS * y0, CONE_RADIUS * z0));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x1, 0, 0));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x1, 0, 0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y1, nn * z1));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y1, nn * z1));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y1, nn * z1));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::float3(x0, 0, 0));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::float3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::float3(x0, CONE_RADIUS * y0, CONE_RADIUS * z0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::float3(-1, 0, 0));
	}

	for (int i = 0; i < 4; ++i)
	{
		s_ArrowTriProj[i].clear();
		s_ArrowTriProj[i].resize(s_ArrowTri[i].size());
		s_ArrowColLight[i].clear();
		s_ArrowColLight[i].resize(s_ArrowTri[i].size());
	}
}

ImU32 blendColor(ImU32 c1, ImU32 c2, float t)
{
	ImColor color1(c1);
	ImColor color2(c2);
	float invt = 1.0f - t;
	color1 = ImColor((color1.Value.x * invt) + (color2.Value.x * t), (color1.Value.y * invt) + (color2.Value.y * t),
					 (color1.Value.z * invt) + (color2.Value.z * t), (color1.Value.w * invt) + (color2.Value.w * t));
	return color1;
}

inline float ImVec2Cross(const ImVec2& left, const ImVec2& right) {
	return (left.x * right.y) - (left.y * right.x);
}

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
	return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y);
}

inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
	return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y);
}

void drawTriangles(ImDrawList* draw_list, const ImVec2& offset, const ImVector<ImVec2>& triProj,
				   const ImVector<ImU32>& colLight, int numVertices)
{
	const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
	assert(numVertices % 3 == 0);
	draw_list->PrimReserve(numVertices, numVertices);
	for (int ii = 0; ii < numVertices / 3; ii++)
	{
		ImVec2 v1 = offset + triProj[ii * 3];
		ImVec2 v2 = offset + triProj[ii * 3 + 1];
		ImVec2 v3 = offset + triProj[ii * 3 + 2];

		// 2D cross product to do culling
		ImVec2 d1 = v2 - v1;
		ImVec2 d2 = v3 - v1;
		float c = ImVec2Cross(d1, d2);
		if (c > 0.0f)
		{
			v2 = v1;
			v3 = v1;
		}

		draw_list->PrimWriteIdx(ImDrawIdx(draw_list->_VtxCurrentIdx));
		draw_list->PrimWriteIdx(ImDrawIdx(draw_list->_VtxCurrentIdx + 1));
		draw_list->PrimWriteIdx(ImDrawIdx(draw_list->_VtxCurrentIdx + 2));
		draw_list->PrimWriteVtx(v1, uv, colLight[ii * 3]);
		draw_list->PrimWriteVtx(v2, uv, colLight[ii * 3 + 1]);
		draw_list->PrimWriteVtx(v3, uv, colLight[ii * 3 + 2]);
	}
}

bool drawArrowWidget(math::float3& direction, float widgetSize, std::uint32_t color)
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	auto directionQuaternion = math::directionToQuaternion(direction);

	if (s_ArrowTri[0].empty())
		initArrowData();

	bool value_changed = false;

	ImVec2 orient_pos = ImGui::GetCursorScreenPos();

	float sv_orient_size = std::min(ImGui::CalcItemWidth(), widgetSize);
	float w = sv_orient_size;
	float h = sv_orient_size;

	// We want to generate quaternion rotations relative to the quaternion in the down press state.
	// This gives us cleaner control over rotation (it feels better)
	static math::quatf origQuat;
	static math::float3 coordOld;
	bool highlighted = false;
	ImGui::InvisibleButton("widget", ImVec2(sv_orient_size, sv_orient_size));
	if (ImGui::IsItemActive())
	{
		highlighted = true;
		ImVec2 mouse = ImGui::GetMousePos() - orient_pos;
		if (ImGui::IsMouseClicked(0))
		{
			origQuat = directionQuaternion;
			coordOld = math::float3(quatIX((int)mouse.x, w, h), quatIY((int)mouse.y, w, h), 1.0f);
		}
		else if (ImGui::IsMouseDragging(0))
		{
			math::float3 coord(quatIX((int)mouse.x, w, h), quatIY((int)mouse.y, w, h), 1.0f);
			math::float3 pVec = coord;
			math::float3 oVec = coordOld;
			coord.z = 0.0f;
			float n0 = length(oVec);
			float n1 = length(pVec);
			if (n0 > math::epsilon<float> && n1 > math::epsilon<float>)
			{
				math::float3 v0 = oVec / n0;
				math::float3 v1 = pVec / n1;
				math::float3 axis = cross(v0, v1);
				float sa = length(axis);
				float ca = dot(v0, v1);
				float angle = atan2(sa, ca);
				if (coord.x * coord.x + coord.y * coord.y > 1.0)
				{
					angle *= 1.0f + 1.5f * (length(coord) - 1.0f);
				}

				math::quatf qrot, qres, qorig;
				qrot = math::quatf::fromAxisAngle(axis, angle);
				float nqorig = sqrt(origQuat.x * origQuat.x + origQuat.y * origQuat.y + origQuat.z * origQuat.z +
									origQuat.w * origQuat.w);
				if (abs(nqorig) > math::epsilon<float> * math::epsilon<float>)
				{
					qorig = origQuat / nqorig;
					qres = qrot * qorig;
					directionQuaternion = qres;
					direction = math::quaternionToDirection(directionQuaternion);
				}
				else
				{
					directionQuaternion = qrot;
					direction = math::quaternionToDirection(directionQuaternion);
				}
				value_changed = true;
			}
		}
		draw_list->AddRectFilled(orient_pos, orient_pos + ImVec2(sv_orient_size, sv_orient_size),
								 ImColor(style.Colors[ImGuiCol_FrameBgActive]), style.FrameRounding);
	}
	else
	{
		ImColor color(ImGui::IsItemHovered() ? style.Colors[ImGuiCol_FrameBgHovered] : style.Colors[ImGuiCol_FrameBg]);
		draw_list->AddRectFilled(orient_pos, orient_pos + ImVec2(sv_orient_size, sv_orient_size), color,
								 style.FrameRounding);
	}

	ImVec2 inner_pos = orient_pos;
	math::quatf quat = normalize(directionQuaternion);
	ImColor alpha(1.0f, 1.0f, 1.0f, highlighted ? 1.0f : 0.75f);
	math::float3 arrowDir = quat * math::float3(1, 0, 0);

	for (int k = 0; k < 4; ++k)
	{
		int j = (arrowDir.z > 0) ? 3 - k : k;
		assert(s_ArrowTriProj[j].size() == (s_ArrowTri[j].size()) &&
			   s_ArrowColLight[j].size() == s_ArrowTri[j].size() && s_ArrowNorm[j].size() == s_ArrowTri[j].size());
		size_t ntri = s_ArrowTri[j].size();
		for (int i = 0; i < ntri; ++i)
		{
			math::float3 coord = s_ArrowTri[j][i];
			math::float3 norm = s_ArrowNorm[j][i];
			if (coord.x > 0)
			{
				coord.x = 2.5f * coord.x - 2.0f;
			}
			else
			{
				coord.x += 0.2f;
			}
			coord.y *= 1.5f;
			coord.z *= 1.5f;
			coord = quat * coord;
			norm = quat * norm;
			s_ArrowTriProj[j][i] = ImVec2(quatPX(coord.x, w, h), quatPY(coord.y, w, h));
			ImU32 col = (color | 0xff000000) & alpha;
			s_ArrowColLight[j][i] = blendColor(0xff000000, col, abs(std::clamp(norm.z, -1.0f, 1.0f)));
		}
		drawTriangles(draw_list, inner_pos, s_ArrowTriProj[j], s_ArrowColLight[j], ntri);
	}

	return value_changed;
}

bool directionWidget(const std::string_view label, math::float3& dir, float size, std::uint32_t color)
{
	ImGui::PushID(label.data());
	ImGui::BeginGroup();

	// DragFloat3 is nicer than SliderFloat3 because it supports double-clicking for keyboard entry.
	bool changed = ImGui::DragFloat3(label.data(), &dir[0], 0.01f, -1.0f, 1.0f);

	auto v = normalize(dir);
	if (drawArrowWidget(v, size, color) && !changed)
	{
		changed = true;
		dir = v;
	}
	ImGui::EndGroup();
	ImGui::PopID();

	return changed;
}

} // namespace spatial::ui