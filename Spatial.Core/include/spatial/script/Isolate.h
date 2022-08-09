#pragma once

#include <v8-isolate.h>

namespace spatial::script
{

class Isolate
{
  public:
	explicit Isolate(const v8::Isolate::CreateParams& params);
	explicit Isolate(v8::Isolate* isolate);

	void reset();

	v8::Isolate* release();

	v8::Isolate* get();

	Isolate(Isolate&& isolate) noexcept;
	Isolate(const Isolate& isolate) noexcept = delete;

	Isolate& operator=(Isolate&& isolate) noexcept;
	Isolate& operator=(const Isolate& isolate) noexcept = delete;

	v8::Isolate* operator->() noexcept
	{
		return mIsolate;
	}

	const v8::Isolate* operator->() const noexcept
	{
		return mIsolate;
	}

	~Isolate();

  private:
	v8::Isolate* mIsolate;
};

} // namespace spatial::script