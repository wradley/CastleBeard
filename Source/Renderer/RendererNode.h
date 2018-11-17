#pragma once
#include "CBMath.h"

namespace Renderer
{
    class Renderer;

    class RendererNode
    {
    public:

        RendererNode(Renderer *renderer, RendererNode *parent);
        ~RendererNode();

        // queue node for deletion in renderer
        virtual void destroy();

        void setLocalTransform(const Math::Transform &t);
        Math::Mat4 calculateGlobalTransformMatrix();
        Math::Quat calculateGlobalOrientation();

    private:

        friend class Scene;

        // parent can sign ID if they want to organize a renderer node in a list
        // and retrieve it quickly
        void assignID(unsigned int i);
        unsigned int getID() const;

    protected:

        Renderer *_renderer;
        RendererNode *_parent;
        unsigned int _id;

        Math::Transform _localTransform;

    private:

        RendererNode(const RendererNode &) = delete;
    };
}