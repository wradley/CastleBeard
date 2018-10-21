#include <atomic>
#include "../../Include/Window/WindowManager.h"
#include "../../Include/Core/Events/EngineEvents.h"
#include "../../Include/Debug.h"


static float scrollOffset = 0.0f;
static bool lMouseDown = true;
static bool rMouseDown = true;
static int lClickTime = 0;
static int rClickTime = 0;
static float lHorizontal = 0.0f;
static float lVertical = 0.0f;
static float rHorizontal = 0.0f;
static float rVertical = 0.0f;

static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    scrollOffset += (float)yOffset;
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


Window::Manager::Manager() : _window(nullptr), _firstUpdate(true)
{}


Window::Manager::~Manager()
{}


void Window::Manager::init(Core::EventManager &em, unsigned int width, unsigned int height)
{
    em.listenFor(Core::EventType::eResizeWindow, &_eventQueue);
    createWindow(width, height);
}


void Window::Manager::update(Core::EventManager & em)
{
    if (_firstUpdate) {
        _firstUpdate = false;

        int width, height;
        glfwGetWindowSize(_window, &width, &height);

        Core::ResizeWindowEvent *rw = new Core::ResizeWindowEvent;
        rw->newWidth = width;
        rw->newHeight = height;
        em.send(std::shared_ptr<const Core::ResizeWindowEvent>(rw), &_eventQueue);
    }

    glfwPollEvents();
    handleEvents();

    // gather and send input event
    Core::ControllerInputEvent *ci = new Core::ControllerInputEvent;
    ci->lHorizontal = lHorizontal;  lHorizontal = 0.0f;
    ci->lVertical   = lVertical;    lVertical = 0.0f;
    ci->rHorizontal = rHorizontal;  rHorizontal = 0.0f;
    ci->rVertical   = rVertical;    rVertical = 0.0f;
    em.send(std::shared_ptr<const Core::ControllerInputEvent>(ci), &_eventQueue);

    // check if we should be shutting down
    if (glfwWindowShouldClose(_window)) {
        auto *sd = new Core::ShutdownEvent;
        em.send(std::shared_ptr<const Core::ShutdownEvent>(sd), &_eventQueue);
    }
}


void Window::Manager::stop(Core::EventManager & em)
{
    handleEvents();
}


void Window::Manager::swapBuffers()
{
    glfwSwapBuffers(_window);
}


void Window::Manager::createWindow(unsigned int width, unsigned int height)
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_LOG("Could not init glad");
        glfwTerminate();
        return;
    }

    // resize for mac retna
    int newWidth, newHeight;
    glfwGetFramebufferSize(_window, &newWidth, &newHeight);
    glViewport(0, 0, newWidth, newHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Window::Manager::handleEvents()
{
    unsigned int size = _eventQueue.size();
    for (unsigned int i = 0; i < size; ++i) {
        auto e = _eventQueue.popFront();
        switch (e->getType()) {
        case Core::EventType::eResizeWindow: handleResizeWindowEvent((Core::ResizeWindowEvent*)e.get()); break;
        default: DEBUG_LOG("Default case"); break;
        }
    }
}


void Window::Manager::handleResizeWindowEvent(Core::ResizeWindowEvent *e)
{}

