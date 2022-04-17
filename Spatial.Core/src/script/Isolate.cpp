#include <spatial/script/Isolate.h>
#include <spatial/script/ScriptSourceStream.h>

namespace spatial::script
{

Isolate::Isolate(v8::Isolate* isolate) : mIsolate{isolate}
{
}

Isolate::Isolate(Isolate&& isolate) noexcept : mIsolate{isolate.release()}
{
}

Isolate::Isolate(const v8::Isolate::CreateParams& params) : Isolate{v8::Isolate::New(params)}
{
}

void Isolate::reset()
{
	if (mIsolate)
		mIsolate->Dispose();
}

v8::Isolate* Isolate::release()
{
	return std::exchange(mIsolate, nullptr);
}

Isolate& Isolate::operator=(Isolate&& isolate) noexcept
{
	reset();
	mIsolate = isolate.release();
	return *this;
}

Isolate::~Isolate()
{
	reset();
}

v8::Isolate* Isolate::get()
{
	return mIsolate;
}

} // namespace spatial::script