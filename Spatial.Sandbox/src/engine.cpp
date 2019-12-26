#include "engine.h"

spatial::core::Application& app()
{
    static spatial::core::Application application;
    return application;
}
