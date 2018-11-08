#include "../../Include/Graphics/RendererNode.h"
#include "../../Include/Graphics/Renderer.h"


Graphics::RendererNode::RendererNode(Renderer *renderer, RendererNode *parent) :
    _renderer(renderer), _parent(parent), _id(0)
{}


Graphics::RendererNode::~RendererNode()
{}


void Graphics::RendererNode::destroy()
{
    _renderer->queueForDeletion(this);
}


void Graphics::RendererNode::setLocalTransform(const Math::Transform &t)
{
    _localTransform = t;
}


Math::Mat4 Graphics::RendererNode::calculateGlobalTransformMatrix()
{
    if (_parent) {
        return _parent->calculateGlobalTransformMatrix() * Math::Mat4(_localTransform);
    }

    return Math::Mat4(_localTransform);
}


Math::Quat Graphics::RendererNode::calculateGlobalOrientation()
{
    if (_parent) {
        return _parent->calculateGlobalOrientation() * _localTransform.orientation;
    }

    return _localTransform.orientation;
}


void Graphics::RendererNode::assignID(unsigned int i)
{
    _id = i;
}


unsigned int Graphics::RendererNode::getID() const
{
    return _id;
}
