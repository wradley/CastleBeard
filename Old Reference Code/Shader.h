#pragma once
#include <string>
#include "glmIncludes.h"
#include "Texture.h"
#include "CubeMap.h"

namespace gp
{
    class Shader
    {
    public:
        
        Shader(
            const std::string &vCode, std::string &vErrRet,
            const std::string &fCode, std::string &fErrRet,
            const std::string &gCode, std::string &gErrRet,
            std::string &linkErrRet
        );
        Shader(Shader &&s);
		Shader(unsigned int glid);
        ~Shader();
        
        void bind();
        
        void setBool(const std::string &name, bool v);
        void setInt(const std::string &name, int v);
        void setFloat(const std::string &name, float v);
        void setVec2(const std::string &name, const glm::vec2 &v);
        void setVec3(const std::string &name, const glm::vec3 &v);
        void setVec4(const std::string &name, const glm::vec4 &v);
        void setMat2(const std::string &name, const glm::mat2 &v);
        void setMat3(const std::string &name, const glm::mat3 &v);
        void setMat4(const std::string &name, const glm::mat4 &v);
        void setTexture(const std::string &name, unsigned int n, const Texture &t);
        void setTexture(const std::string &name, unsigned int n, unsigned int glID);
		void setCubeMap(const std::string &name, unsigned int n, const CubeMap &c);
        
    private:
        
		bool _valid;
        unsigned int _id;
        
    private:
        
        Shader(const Shader &) = delete;
        Shader& operator= (const Shader &) = delete;
        
    };
}
