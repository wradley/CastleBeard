#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CBEvent.h"

namespace Window
{
    class Manager
    {
    public:

        Manager();
        ~Manager();

        void init(Engine::EventManager &em, unsigned int width, unsigned int height);
        void update(Engine::EventManager &em);
        void stop(Engine::EventManager &em);

        void swapBuffers();

    private:

        void createWindow(unsigned int width, unsigned int height);
        void handleEvents();
        void handleResizeWindowEvent(Engine::ResizeWindowEvent *e);

        GLFWwindow *_window;
        int _width;
        int _height;
        Engine::EventQueue _eventQueue;
    };
}