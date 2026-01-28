#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
namespace engine
{
    bool isOpenGLInitialized();
    void initializeFreeType();
    FT_Library &getFreeTypeLibrary();
    void initializeOpenGL(int major = 3, int minor = 3 );
    void terminateOpenGL();
} // namespace engine