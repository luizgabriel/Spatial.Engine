#include "engine.h"

spatial::core::Application& app()
{
    static Application application;
    return application;
}
