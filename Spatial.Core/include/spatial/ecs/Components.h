#include <spatial/common/Math.h>
#include <variant>

namespace spatial
{

struct TransformComponent
{
	math::float3 position;
	math::float3 scale;
	math::float3 rotation;
};

struct CameraBaseProjection
{
	double near;
	double far;

	constexpr CameraBaseProjection(double near, double far) : near{near}, far{far}
	{
	}
};

struct PerspectiveProjection : public CameraBaseProjection
{
	double fieldOfView;
	double aspectRatio;

	constexpr PerspectiveProjection(double fieldOfView, double aspectRatio, double near, double far)
		: CameraBaseProjection(near, far), fieldOfView{fieldOfView}, aspectRatio{aspectRatio}
	{
	}
};

struct OrthographicProjection : public CameraBaseProjection
{
	double left;
	double right;
	double bottom;
	double top;

	constexpr OrthographicProjection(double left, double right, double bottom, double top, double near, double far)
		: CameraBaseProjection(near, far), left{left}, right{right}, bottom{bottom}, top{top}
	{
	}

	constexpr OrthographicProjection(double aspectRatio, double near, double far)
		: OrthographicProjection(-aspectRatio, aspectRatio, -1, 1, near, far)
	{
	}
};

struct CustomProjection : public CameraBaseProjection
{
	math::mat4 projectionMatrix;

	constexpr CustomProjection(math::mat4 projectionMatrix, double near, double far)
		: CameraBaseProjection(near, far), projectionMatrix{std::move(projectionMatrix)}
	{
	}
};

using CameraProjection = std::variant<PerspectiveProjection, OrthographicProjection, CustomProjection>;

struct CameraComponent
{
	CameraProjection projection;
};

}