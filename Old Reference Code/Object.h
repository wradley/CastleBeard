#pragma once
#include "Transform.h"

namespace gp
{
    class Object
    {
    public:
        
        void setPosition(float x, float y, float z);
        void setRotation(float x, float y, float z);
        void setRotationQuat(float x, float y, float z, float w);
        void setScale(float x, float y, float z);
        
    protected:
        
        Transform _transform;
        
    };
}
