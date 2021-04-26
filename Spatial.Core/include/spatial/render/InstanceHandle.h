#pragma once

#include <memory>
#include <spatial/render/Stage.h>
#include <type_traits>

namespace spatial
{

namespace detail
{
template <typename StageType>
class InstanceHandleBase
{
  public:
	constexpr InstanceHandleBase(StageType& stage, Instance instance) : mStage{stage}, mInstance{instance}
	{
	}

	InstanceHandleBase(const InstanceHandleBase& other) : InstanceHandleBase(other.mStage, other.mInstance)
	{
	}

	InstanceHandleBase& operator=(const InstanceHandleBase& other)
	{
		assert(&mStage == &other.mStage);
		mInstance = other.mInstance;

		return *this;
	}

	void destroy()
	{
		assert(isValid());
		mStage.getRegistry().destroy(mInstance);
	}

	bool isValid() const noexcept
	{
		return mStage.getRegistry().valid(mInstance);
	}

	template <typename Component, typename... Args>
	Component& add(Args&&... args)
	{
		assert(isValid());
		return mStage.getRegistry().template emplace<Component>(mInstance, std::forward<Args>(args)...);
	}

	template <typename Component>
	Component& get()
	{
		assert(isValid());
		return mStage.getRegistry().template get<Component>(mInstance);
	}

	template <typename Component>
	const Component& get() const
	{
		assert(isValid());
		return mStage.getRegistry().template get<const Component>(mInstance);
	}

	template <typename Component, typename... Args>
	const Component& addOrGet(Args&&... args) const
	{
		assert(isValid());
		return mStage.getRegistry().template emplace_or_replace<Component>(mInstance, std::forward<Args>(args)...);
	}

	template <typename Component>
	bool has() const
	{
		assert(isValid());
		return mStage.getRegistry().template all_of<Component>(mInstance);
	}

	template <typename Component>
	void remove()
	{
		assert(isValid());
		mStage.getRegistry().template remove<Component>(mInstance);
	}

	bool operator==(Instance rhs) const noexcept
	{
		return mInstance == rhs;
	}

	bool operator!=(Instance rhs) const noexcept
	{
		return mInstance != rhs;
	}

	operator bool() const noexcept
	{
		return isValid();
	}

	operator Instance() const noexcept
	{
		return mInstance;
	}

	operator Instance&() noexcept
	{
		return mInstance;
	}

  private:
	StageType& mStage;
	Instance mInstance;
};

template <typename Instance>
InstanceHandleBase(spatial::Stage&, Instance) -> InstanceHandleBase<spatial::Stage>;

template <typename Instance>
InstanceHandleBase(const spatial::Stage&, Instance) -> InstanceHandleBase<const spatial::Stage>;

} // namespace detail

using InstanceHandle = detail::InstanceHandleBase<spatial::Stage>;
using ConstInstanceHandle = const detail::InstanceHandleBase<const spatial::Stage>;

template <typename StageType>
constexpr auto handleOf(StageType& stage, Instance instance)
{
	return detail::InstanceHandleBase{stage, instance};
}

} // namespace spatial