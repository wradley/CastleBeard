#pragma once
#include <string>
#include <glad/glad.h>
#include "../Math/Vector.h"
#include "../Math/Matrix.h"

namespace Graphics
{
    class Shader
    {
    public:

        Shader();
        ~Shader();

        bool compileFiles(const char *vertexCode, const char *fragCode);

        void bind();

        void setInt(const std::string &name, int v);
        void setFloat(const std::string &name, float v);
        void setVec2(const std::string &name, const Math::Vec2 &v);
        void setVec3(const std::string &name, const Math::Vec3 &v);
        void setVec4(const std::string &name, const Math::Vec4 &v);
        void setMat4(const std::string &name, const Math::Mat4 &v);

    private:

        unsigned int _id;

    private:

        Shader(Shader &&) = delete;
        Shader(const Shader &) = delete;
    };
}