#pragma once
namespace engine
{
    bool isOpenGLInitialized();
    void initializeOpenGL(int major = 3, int minor = 3 );
    void terminateOpenGL();
} // namespace engine