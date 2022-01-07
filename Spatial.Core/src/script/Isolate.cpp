#include <spatial/script/Isolate.h>

namespace spatial::script
{

Isolate::Isolate(v8::Isolate* isolate) : mIsolate{isolate}
{
}

Isolate::Isolate(const v8::Isolate::CreateParams& params) : Isolate{v8::Isolate::New(params)}
{
}

Isolate::~Isolate()
{
	mIsolate->Dispose();
}

}