#include "../../../Include/Graphics/Environment/Camera.h"

Graphics::Camera::Camera() : 
    node(0), nearPlane(0.1f), farPlane(1000.0f), 
    fov(60.0f), aspectRatio(1.6667f)
{}
