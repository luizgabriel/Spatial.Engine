#include <spatial/common/EBus.h>

namespace spatial::common
{

common::EventQueue &EBus::get() noexcept
{
    static common::EventQueue queue;
    return queue;
}

}