//#include <atomic>
#include "WindowManager.h"
#include "Debug.h"


static float scrollOffset = 0.0f;
static bool lMouseDown = true;
static bool rMouseDown = true;
static int lClickTime = 0;
static int rClickTime = 0;
static float mouseHorizontal = 0.0f;
static float mouseVertical = 0.0f;
static bool wKey = false;
static bool aKey = false;
static bool sKey = false;
static bool dKey = false;
static bool escKey = false;


static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    static double lastX = xpos;
    static double lastY = ypos;

    mouseHorizontal += (float)(xpos - lastX);
    mouseVertical += -(float)(ypos - lastY);

    lastX = xpos;
    lastY = ypos;
}


static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    scrollOffset += (float)yOffset;
}


static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS) wKey = true;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) aKey = true;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) sKey = true;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) dKey = true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) escKey = true;

    if (key == GLFW_KEY_W && action == GLFW_RELEASE) wKey = false;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) aKey = false;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) sKey = false;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) dKey = false;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) escKey = false;
}


static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS) lMouseDown = true;
        if (action == GLFW_RELEASE) {
            lMouseDown = false; 
            lClickTime = 0;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS) rMouseDown = true;
        if (action == GLFW_RELEASE) {
            rMouseDown = false;
            rClickTime = 0;
        }
    }
}


namespace Window
{
    Manager::Manager() :
        _window(nullptr), _width(0), _height(0)
    {}


    Manager::~Manager()
    {}


    void Manager::init(Engine::EventManager &em, unsigned int width, unsigned int height)
    {
        em.listenFor(Engine::EventType::eResizeWindow, &_eventQueue);
        createWindow(width, height);
    }


    void Manager::update(Engine::EventManager & em)
    {
        // check for window resize
        int nWidth, nHeight;
        glfwGetFramebufferSize(_window, &nWidth, &nHeight);
        if (nWidth != _width || nHeight != _height) {
            Engine::ResizeWindowEvent *rw = new Engine::ResizeWindowEvent;
            rw->newWidth = nWidth;
            rw->newHeight = nHeight;
            em.send(std::shared_ptr<const Engine::ResizeWindowEvent>(rw), &_eventQueue);
        }

        glfwPollEvents();
        handleEvents();

        // gather and send input event
        if (mouseHorizontal != 0.0f || mouseVertical != 0.0f ||
            wKey || aKey || sKey || dKey || escKey
            ) {
            Engine::ControllerInputEvent *ci = new Engine::ControllerInputEvent;
            ci->lHorizontal = mouseHorizontal;
            mouseHorizontal = 0.0f;
            ci->lVertical = mouseVertical;
            mouseVertical = 0.0f;
            ci->rHorizontal = 0.0f;
            ci->rVertical = 0.0f;
            if (wKey) ci->rVertical += 1.0f;
            if (sKey) ci->rVertical -= 1.0f;
            if (aKey) ci->rHorizontal -= 1.0f;
            if (dKey) ci->rHorizontal += 1.0f;
            ci->pause = escKey;
            em.send(std::shared_ptr<const Engine::ControllerInputEvent>(ci), &_eventQueue);
        }

        // check if we should be shutting down
        if (glfwWindowShouldClose(_window)) {
            auto *sd = new Engine::ShutdownEvent;
            em.send(std::shared_ptr<const Engine::ShutdownEvent>(sd), &_eventQueue);
        }
    }


    void Manager::stop(Engine::EventManager & em)
    {
        handleEvents();
    }


    void Manager::swapBuffers()
    {
        glfwSwapBuffers(_window);
    }


    void Manager::createWindow(unsigned int width, unsigned int height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        _window = glfwCreateWindow(width, height, "CMakeTest", NULL, NULL);
        if (!_window) {
            DEBUG_LOG("Could not create glfw window");
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(_window);
        glfwSetMouseButtonCallback(_window, MouseButtonCallback);
        glfwSetScrollCallback(_window, ScrollCallback);
        glfwSetCursorPosCallback(_window, CursorPositionCallback);
        glfwSetKeyCallback(_window, KeyCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            DEBUG_LOG("Could not init glad");
            glfwTerminate();
            return;
        }

        // resize for mac retna
        int newWidth, newHeight;
        glfwGetFramebufferSize(_window, &newWidth, &newHeight);



        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }


    void Manager::handleEvents()
    {
        unsigned int size = _eventQueue.size();
        for (unsigned int i = 0; i < size; ++i) {
            auto e = _eventQueue.popFront();
            switch (e->getType()) {
            case Engine::EventType::eResizeWindow: handleResizeWindowEvent((Engine::ResizeWindowEvent*)e.get()); break;
            default: DEBUG_LOG("Default case"); break;
            }
        }
    }


    void Manager::handleResizeWindowEvent(Engine::ResizeWindowEvent *e)
    {
        DEBUG_LOG("Not Impl'd");
    }

}