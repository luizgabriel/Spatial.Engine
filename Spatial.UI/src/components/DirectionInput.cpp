#include <imgui.h>
#include <spatial/ui/components/DirectionInput.h>
#include <spatial/ui/components/VectorInput.h>
#include <algorithm>

namespace spatial::ui
{

float quatD(float w, float h)
{
	return std::min(std::abs(w), std::abs(h)) - 4.0F;
}

float quatPX(float x, float w, float h)
{
	return (x * 0.5f * quatD(w, h) + w * 0.5f + 0.5F);
}

float quatPY(float y, float w, float h)
{
	return (-y * 0.5f * quatD(w, h) + h * 0.5f - 0.5f);
}

float quatIX(float x, float w, float h)
{
	return (2.0f * x - w - 1.0f) / quatD(w, h);
}

float quatIY(float y, float w, float h)
{
	return (-2.0f * y + h - 1.0f) / quatD(w, h);
}

inline float ImVec2Cross(const ImVec2& left, const ImVec2& right)
{
	return (left.x * right.y) - (left.y * right.x);
}

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
{
	return {lhs.x - rhs.x, lhs.y - rhs.y};
}

static ImVector<math::vec3> s_ArrowTri[4];
static ImVector<ImVec2> s_ArrowTriProj[4];
static ImVector<math::vec3> s_ArrowNorm[4];
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
		a0 = 2.0f * math::pi * (float(i)) / SUBDIV;
		a1 = 2.0f * math::pi * (float(i + 1)) / SUBDIV;
		x0 = ARROW_BGN;
		x1 = ARROW_END - CONE_LENGTH;
		y0 = cosf(a0);
		z0 = sinf(a0);
		y1 = cosf(a1);
		z1 = sinf(a1);
		s_ArrowTri[ARROW_CYL].push_back(math::vec3(x1, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::vec3(x0, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::vec3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CYL].push_back(math::vec3(x1, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::vec3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CYL].push_back(math::vec3(x1, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::vec3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::vec3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::vec3(0, y1, z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::vec3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::vec3(0, y1, z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::vec3(0, y1, z1));
		s_ArrowTri[ARROW_CAP].push_back(math::vec3(x0, 0, 0));
		s_ArrowTri[ARROW_CAP].push_back(math::vec3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CAP].push_back(math::vec3(x0, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowNorm[ARROW_CAP].push_back(math::vec3(-1, 0, 0));
		s_ArrowNorm[ARROW_CAP].push_back(math::vec3(-1, 0, 0));
		s_ArrowNorm[ARROW_CAP].push_back(math::vec3(-1, 0, 0));
		x0 = ARROW_END - CONE_LENGTH;
		x1 = ARROW_END;
		nx = CONE_RADIUS / (x1 - x0);
		nn = 1.0f / sqrtf(nx * nx + 1);
		s_ArrowTri[ARROW_CONE].push_back(math::vec3(x1, 0, 0));
		s_ArrowTri[ARROW_CONE].push_back(math::vec3(x0, CONE_RADIUS * y0, CONE_RADIUS * z0));
		s_ArrowTri[ARROW_CONE].push_back(math::vec3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE].push_back(math::vec3(x1, 0, 0));
		s_ArrowTri[ARROW_CONE].push_back(math::vec3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE].push_back(math::vec3(x1, 0, 0));
		s_ArrowNorm[ARROW_CONE].push_back(math::vec3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::vec3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::vec3(nn * nx, nn * y1, nn * z1));
		s_ArrowNorm[ARROW_CONE].push_back(math::vec3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::vec3(nn * nx, nn * y1, nn * z1));
		s_ArrowNorm[ARROW_CONE].push_back(math::vec3(nn * nx, nn * y1, nn * z1));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::vec3(x0, 0, 0));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::vec3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::vec3(x0, CONE_RADIUS * y0, CONE_RADIUS * z0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::vec3(-1, 0, 0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::vec3(-1, 0, 0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::vec3(-1, 0, 0));
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

void drawTriangles(ImDrawList* drawList, const ImVec2& offset, const ImVector<ImVec2>& triProj,
				   const ImVector<ImU32>& colLight, int numVertices)
{
	const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
	assert(numVertices % 3 == 0);
	drawList->PrimReserve(numVertices, numVertices);
	for (int ii = 0; ii < numVertices / 3; ii++)
	{
		ImVec2 v1 = offset + triProj[ii * 3];
		ImVec2 v2 = offset + triProj[ii * 3 + 1];
		ImVec2 v3 = offset + triProj[ii * 3 + 2];

		// 2D cross product to do culling
		ImVec2 d1 = v2 - v1;
		ImVec2 d2 = v3 - v1;
		float c = ImVec2Cross(d1, d2);
		if (c > 0.0F)
		{
			v2 = v1;
			v3 = v1;
		}

		drawList->PrimWriteIdx(ImDrawIdx(drawList->_VtxCurrentIdx));
		drawList->PrimWriteIdx(ImDrawIdx(drawList->_VtxCurrentIdx + 1));
		drawList->PrimWriteIdx(ImDrawIdx(drawList->_VtxCurrentIdx + 2));
		drawList->PrimWriteVtx(v1, uv, colLight[ii * 3]);
		drawList->PrimWriteVtx(v2, uv, colLight[ii * 3 + 1]);
		drawList->PrimWriteVtx(v3, uv, colLight[ii * 3 + 2]);
	}
}

bool drawArrowWidget(math::vec3& direction, float widgetSize, std::uint32_t color)
{
	auto& style = ImGui::GetStyle();
	auto* drawList = ImGui::GetWindowDrawList();
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
	static math::quat origQuat;
	static math::vec3 coordOld;
	bool highlighted = false;
	ImGui::InvisibleButton("widget", ImVec2(sv_orient_size, sv_orient_size));
	if (ImGui::IsItemActive())
	{
		highlighted = true;
		ImVec2 mouse = ImGui::GetMousePos() - orient_pos;
		if (ImGui::IsMouseClicked(0))
		{
			origQuat = directionQuaternion;
			coordOld = math::vec3(quatIX(mouse.x, w, h), quatIY(mouse.y, w, h), 1.0f);
		}
		else if (ImGui::IsMouseDragging(0))
		{
			math::vec3 coord(quatIX(mouse.x, w, h), quatIY(mouse.y, w, h), 1.0f);
			math::vec3 pVec = coord;
			math::vec3 oVec = coordOld;
			coord.z = 0.0f;
			float n0 = length(oVec);
			float n1 = length(pVec);
			if (n0 > math::epsilon && n1 > math::epsilon)
			{
				math::vec3 v0 = oVec / n0;
				math::vec3 v1 = pVec / n1;
				math::vec3 axis = cross(v0, v1);
				float sa = length(axis);
				float ca = dot(v0, v1);
				float angle = atan2(sa, ca);
				if (coord.x * coord.x + coord.y * coord.y > 1.0f)
				{
					angle *= 1.0f + 1.5f * (length(coord) - 1.0f);
				}

				math::quat qrot;
				math::quat qres;
				math::quat qorig;
				
				qrot = math::angleAxis(angle, axis);
				float nqorig = sqrt(origQuat.x * origQuat.x + origQuat.y * origQuat.y + origQuat.z * origQuat.z
									+ origQuat.w * origQuat.w);
				if (abs(nqorig) > math::epsilon * math::epsilon)
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
		drawList->AddRectFilled(orient_pos, orient_pos + ImVec2(sv_orient_size, sv_orient_size),
								ImColor(style.Colors[ImGuiCol_FrameBgActive]), style.FrameRounding);
	}
	else
	{
		auto imcolor =
			ImColor{ImGui::IsItemHovered() ? style.Colors[ImGuiCol_FrameBgHovered] : style.Colors[ImGuiCol_FrameBg]};
		drawList->AddRectFilled(orient_pos, orient_pos + ImVec2(sv_orient_size, sv_orient_size), imcolor,
								style.FrameRounding);
	}

	auto innerPos = orient_pos;
	auto quat = normalize(directionQuaternion);
	auto alpha = ImColor{1.0f, 1.0f, 1.0f, highlighted ? 1.0f : 0.75f};
	auto arrowDir = quat * math::axisX;

	for (int k = 0; k < 4; ++k)
	{
		int j = (arrowDir.z > 0) ? 3 - k : k;
		assert(s_ArrowTriProj[j].size() == (s_ArrowTri[j].size()) && s_ArrowColLight[j].size() == s_ArrowTri[j].size()
			   && s_ArrowNorm[j].size() == s_ArrowTri[j].size());

		auto ntri = s_ArrowTri[j].size();
		for (int i = 0; i < ntri; ++i)
		{
			math::vec3 coord = s_ArrowTri[j][i];
			math::vec3 norm = s_ArrowNorm[j][i];
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
		drawTriangles(drawList, innerPos, s_ArrowTriProj[j], s_ArrowColLight[j], ntri);
	}

	return value_changed;
}

bool directionInput(const std::string_view label, math::vec3& dir, float size, std::uint32_t color)
{
	ImGui::PushID(label.data());
	ImGui::BeginGroup();

	// DragFloat3 is nicer than SliderFloat3 because it supports double-clicking for keyboard entry.
	bool changed = vec3Input(label.data(), dir, 1.0f, 0.01f, -1.0f, 1.0f);

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