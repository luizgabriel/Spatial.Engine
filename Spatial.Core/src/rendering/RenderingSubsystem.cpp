#include <spatial/rendering/RenderingSubsystem.h>

namespace spatial
{

RenderingSubsystem::RenderingSubsystem(desktop::Window &window)
    : m_mainWindow{window}
{
    m_mainWindow.makeCurrentContext();
    glClearColor(0x00, 0x33, 0x66, 0xFF);
}

void RenderingSubsystem::onStartFrame()
{
    auto [width, height] = m_mainWindow.getFrameBufferSize();

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

void RenderingSubsystem::onEndFrame()
{
    m_mainWindow.swapBuffers();
}

} // namespace spatial