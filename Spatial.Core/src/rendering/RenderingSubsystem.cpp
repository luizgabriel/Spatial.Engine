#include <spatial/rendering/RenderingSubsystem.h>

namespace spatial
{

RenderingSubsystem::RenderingSubsystem()
    : m_windowContext(),
      m_mainWindow(m_windowContext.createWindow(1280, 720, "Spatial Engine"))
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

void RenderingSubsystem::pollWindowEvents()
{
    m_windowContext.pollEvents();
}

void RenderingSubsystem::onEndFrame()
{
    m_mainWindow.swapBuffers();
}

} // namespace spatial