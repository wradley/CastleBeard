#include "RendererNode.h"
#include "Renderer.h"


namespace Renderer
{
    RendererNode::RendererNode(Renderer *renderer, RendererNode *parent) :
        _renderer(renderer), _parent(parent), _id(0)
    {}


    RendererNode::~RendererNode()
    {}


    void RendererNode::destroy()
    {
        _renderer->queueForDeletion(this);
    }


    void RendererNode::setLocalTransform(const Math::Transform &t)
    {
        _localTransform = t;
    }


    Math::Mat4 RendererNode::calculateGlobalTransformMatrix()
    {
        if (_parent) {
            return _parent->calculateGlobalTransformMatrix() * Math::Mat4(_localTransform);
        }

        return Math::Mat4(_localTransform);
    }


    Math::Quat RendererNode::calculateGlobalOrientation()
    {
        if (_parent) {
            return _parent->calculateGlobalOrientation() * _localTransform.orientation;
        }

        return _localTransform.orientation;
    }


    void RendererNode::assignID(unsigned int i)
    {
        _id = i;
    }


    unsigned int RendererNode::getID() const
    {
        return _id;
    }
}
