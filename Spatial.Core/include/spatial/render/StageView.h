#pragma once

#include <entt/entity/registry.hpp>

namespace spatial
{

template <typename... Type>
using ExcludeComponent = entt::exclude_t<Type...>;

template <typename... Types>
class StageView
{
  public:
	using BaseView = entt::view<Types...>;

	class Iterator
	{
	  public:
		using BaseIterator = typename BaseView::iterator;

		Iterator(entt::registry* registry, BaseIterator iterator) : mRegistry{registry}, mIterator{iterator}
		{
		}

		Iterator& operator++() noexcept
		{
			return mIterator++, *this;
		}

		[[nodiscard]] bool operator==(const Iterator &other) const noexcept {
			return other.mIterator == mIterator;
		}

		[[nodiscard]] bool operator!=(const Iterator &other) const noexcept {
			return mIterator != other.mIterator;
		}

		[[nodiscard]] Actor operator*() const noexcept {
			return {mRegistry, *mIterator};
		}

	  private:
		entt::registry* mRegistry;
		BaseIterator mIterator;
	};

	StageView(entt::registry* registry, BaseView view) : mRegistry{registry}, mView{view}
	{
	}

	Iterator begin()
	{
		return {mRegistry, mView.begin()};
	}

	Iterator end()
	{
		return {mRegistry, mView.end()};
	}

	template <typename Function>
	void forEach(Function function)
	{
		mView.each([&](auto&... components) { function(components...); });
	}

  private:
	entt::registry* mRegistry;
	BaseView mView;
};

} // namespace spatial