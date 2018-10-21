#include "../../../Include/Graphics/Environment/EnvironmentNode.h"


Graphics::EnvironmentNode::EnvironmentNode(unsigned int id) : ID(id), parent(nullptr)
{}


Graphics::EnvironmentNode::~EnvironmentNode()
{}


Math::Mat4 Graphics::EnvironmentNode::getWorldTransformMatrix() const
{
    if (parent) {
        return parent->getWorldTransformMatrix() * Math::Mat4(transform);
    }

    return Math::Mat4(transform);
}


Math::Quat Graphics::EnvironmentNode::getWorldRotation() const
{
    if (parent) {
        return parent->getWorldRotation() * transform.rotation;
    }

    return transform.rotation;
}


void Graphics::EnvironmentNode::deleteChildren(std::vector<unsigned int>& deletedIDs)
{
    for (EnvironmentNode *child : children) {
        child->deleteChildren(deletedIDs);
        deletedIDs.push_back(child->ID);
        delete child;
    }
}
