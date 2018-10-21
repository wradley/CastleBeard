#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Core/EventManager.h"
#include "../../Include/Core/Events/InputEvents.h"
#include "../../Include/Core/Events/WindowEvents.h"

namespace Window
{
    class Manager
    {
    public:

        Manager();
        ~Manager();

        void init(Core::EventManager &em, unsigned int width, unsigned int height);
        void update(Core::EventManager &em);
        void stop(Core::EventManager &em);

        void swapBuffers();

    private:

        void createWindow(unsigned int width, unsigned int height);
        void handleEvents();
        void handleResizeWindowEvent(Core::ResizeWindowEvent *e);

        GLFWwindow *_window;
        bool _firstUpdate;
        Core::EventQueue _eventQueue;
    };
}