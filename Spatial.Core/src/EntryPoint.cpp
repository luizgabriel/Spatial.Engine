#include <spatial/EntryPoint.h>
#include <spatial/Application.h>

namespace spatial
{

int startEngine()
{
    Application app;
    return app.run();
}

} // namespace spatial