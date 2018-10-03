
#include "Window.h"
#include <iostream>


static int _width, _height;
static bool _hideCursor;
static GLFWwindow *_window;

static std::vector<gp::Window::Key> _keysPressed;
static std::vector<gp::Window::Key> _keysReleased;
static double _horizontalMouse;
static double _verticalMouse;
static double _lastMouseX;
static double _lastMouseY;
static bool _mouseHasInit;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    _width = width;
    _height = height;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) _keysPressed.push_back((gp::Window::Key)key);
    if (action == GLFW_RELEASE) _keysReleased.push_back((gp::Window::Key)key);
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!_mouseHasInit) {
        _mouseHasInit = true;
        _lastMouseX = xpos;
        _lastMouseY = ypos;
    }
    
    _horizontalMouse += xpos - _lastMouseX;
    _verticalMouse   += ypos - _lastMouseY;
    _lastMouseX = xpos;
    _lastMouseY = ypos;
}


bool gp::Window::Open(const std::string &title, int width, int height)
{
    _width = width;
    _height = height;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!_window) {
        std::cout << "Could not create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(_window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not init glad" << std::endl;
        return false;
    }
    
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(_window, cursor_position_callback);
    glfwSetKeyCallback(_window, key_callback);
    
    glfwGetFramebufferSize(_window, &_width, &_height);
    glViewport(0, 0, _width, _height);
    
    _hideCursor = false;
    _horizontalMouse = 0.0;
    _verticalMouse = 0.0;
    _lastMouseX = 0.0;
    _lastMouseY = 0.0;
    _mouseHasInit = false;
    
    return true;
}


void gp::Window::Close()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}


int gp::Window::GetWidth()
{
    return _width;
}


int gp::Window::GetHeight()
{
    return _height;
}


bool gp::Window::ShouldClose()
{
    return glfwWindowShouldClose(_window);
}


void gp::Window::PollEvents()
{
    glfwPollEvents();
}


void gp::Window::SwapBuffers()
{
    glfwSwapBuffers(_window);
}


void gp::Window::VSyncOn()
{
    glfwSwapInterval(1);
}


void gp::Window::VSyncOff()
{
    glfwSwapInterval(0);
}


void gp::Window::HideCursor()
{
    _hideCursor = true;
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void gp::Window::ShowCursor()
{
    _hideCursor = false;
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void gp::Window::SetFullScreen(int width, int height)
{
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

	glfwGetFramebufferSize(_window, &_width, &_height);
}


void gp::Window::SetWindowed(int width, int height)
{
	glfwSetWindowMonitor(
		_window, NULL,
		100, 100, width, height,
		GLFW_DONT_CARE
	);

	glfwGetFramebufferSize(_window, &_width, &_height);
}


std::vector<gp::Window::Key> gp::Window::KeysPressed()
{
    std::vector<gp::Window::Key> ret = _keysPressed;
    _keysPressed.clear();
    return ret;
}


std::vector<gp::Window::Key> gp::Window::KeysReleased()
{
    std::vector<gp::Window::Key> ret = _keysReleased;
    _keysReleased.clear();
    return ret;
}


double gp::Window::MouseHorizontal()
{
    double dst = _horizontalMouse;
    _horizontalMouse = 0.0;
    return dst;
}


double gp::Window::MouseVertical()
{
    double dst = _verticalMouse;
    _verticalMouse = 0.0;
    return dst;
}










