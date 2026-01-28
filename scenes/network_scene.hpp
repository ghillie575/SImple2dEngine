#pragma once
#include <object.hpp>
#include <physics/physics.hpp>
#include <input.hpp>
namespace engine
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        
        // Lifecycle methods
        void initialize();
        void render();
        void onWindowResize(int width, int height);
        void onMouseClick(double x, double y);
        void onMouseMove(double x, double y);
        void onMouseRelease();
        void onKeyPress(Key key);
        
        
    private:
        
        int windowWidth;
        int windowHeight;
    };
} // namespace engine
