#pragma once
#include "RendererNode.h"
#include "../Math/Matrix.h"

namespace Graphics
{
    class Renderer;
    class Scene;

    class Camera : public RendererNode
    {
    public:

        Camera(Renderer *renderer, Scene *parent);
        ~Camera();

        void destroy() override;

        void setFarPlane(float v);
        void setNearPlane(float v);
        void setAspectRatio(float v);
        void setFieldOfView(float v);

        float getFarPlane() const;
        float getNearPlane() const;
        float getAspectRatio() const;
        float getFieldOfView() const;

        Math::Mat4 calculatePerspectiveMatrix();
        Math::Mat4 calculateViewMatrix();

    private:

        float _nearPlane;
        float _farPlane;
        float _fov;
        float _aspectRatio;

    };
}