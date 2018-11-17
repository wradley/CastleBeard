#include "Camera.h"
#include "Scene.h"
#include "CBMath.h"

namespace Renderer
{
    Camera::Camera(Renderer *renderer, Scene *parent) :
        RendererNode(renderer, (RendererNode*)parent),
        _nearPlane(0.1f), _farPlane(1000.0f),
        _fov(60.0f), _aspectRatio(1.6667f)
    {}


    void Camera::destroy()
    {
        ((Scene*)_parent)->forgetChildCamera(this);
        RendererNode::destroy();
    }


    void Camera::setFarPlane(float v)
    {
        _farPlane = v;
    }


    void Camera::setNearPlane(float v)
    {
        _nearPlane = v;
    }


    void Camera::setAspectRatio(float v)
    {
        _aspectRatio = v;
    }


    void Camera::setFieldOfView(float v)
    {
        _fov = v;
    }


    float Camera::getFarPlane() const
    {
        return _farPlane;
    }


    float Camera::getNearPlane() const
    {
        return _nearPlane;
    }


    float Camera::getAspectRatio() const
    {
        return _aspectRatio;
    }


    float Camera::getFieldOfView() const
    {
        return _fov;
    }


    Math::Mat4 Camera::calculatePerspectiveMatrix()
    {
        return Math::Perspective(Math::ToRadians(_fov), _aspectRatio, _nearPlane, _farPlane);
    }


    Math::Mat4 Camera::calculateViewMatrix()
    {
        Math::Mat4 trasform = calculateGlobalTransformMatrix();
        Math::Quat orientation = calculateGlobalOrientation();
        Math::Vec3 pos(trasform * Math::Vec4(_localTransform.position, 1.0f));
        Math::Vec3 forward(orientation * Math::Vec3(0.0f, 0.0f, -1.0f));
        Math::Vec3 up(orientation * Math::Vec3(0.0f, 1.0f, 0.0f));
        return Math::LookAt(pos, pos + Math::Normalize(forward), Math::Normalize(up));
    }
}