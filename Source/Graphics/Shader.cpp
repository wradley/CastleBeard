#include <iostream>
#include "../../Include/Graphics/Shader.h"


Graphics::Shader::Shader() : _id(0)
{}


Graphics::Shader::~Shader()
{
    glDeleteProgram(_id);
}


bool Graphics::Shader::compileFiles(const char *vertexCode, const char *geomCode, const char *fragCode)
{
    int success;
    char infoLog[1024];

    // vertex
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexCode, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader, 1024, NULL, infoLog);
        std::cout << "Error compiling vertex shader: " << infoLog << std::endl;
        return false;
    }

    // geometry shader
    unsigned int gShader;
    if (geomCode) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geomCode, NULL);
        glCompileShader(gShader);
        glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(gShader, 1024, NULL, infoLog);
            std::cout << "Error compiling geometry shader: " << infoLog << std::endl;
            glDeleteShader(vShader);
            return false;
        }
    }

    // fragment
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragCode, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, 1024, NULL, infoLog);
        std::cout << "Error compiling fragment shader: " << infoLog << std::endl;
        glDeleteShader(vShader);
        if (geomCode) glDeleteShader(gShader);
        return false;
    }

    _id = glCreateProgram();
    glAttachShader(_id, vShader);
    if (geomCode) glAttachShader(_id, gShader);
    glAttachShader(_id, fShader);
    if (gShader) glAttachShader(_id, fShader);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, 1024, NULL, infoLog);
        std::cout << "Error linking shader program: " << infoLog << std::endl;
        glDeleteShader(vShader);
        if (geomCode) glDeleteShader(gShader);
        glDeleteShader(fShader);
        return false;
    }

    glDeleteShader(vShader);
    if (geomCode) glDeleteShader(gShader);
    glDeleteShader(fShader);
    return true;
}


void Graphics::Shader::bind()
{
    glUseProgram(_id);
}


void Graphics::Shader::setInt(const std::string & name, int v)
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), v);
}


void Graphics::Shader::setFloat(const std::string & name, float v)
{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), v);
}


void Graphics::Shader::setVec2(const std::string & name, const Math::Vec2 & v)
{
    glUniform2f(glGetUniformLocation(_id, name.c_str()), v.x, v.y);
}


void Graphics::Shader::setVec3(const std::string & name, const Math::Vec3 & v)
{
    glUniform3f(glGetUniformLocation(_id, name.c_str()), v.x, v.y, v.z);
}


void Graphics::Shader::setVec4(const std::string & name, const Math::Vec4 & v)
{
    glUniform4f(glGetUniformLocation(_id, name.c_str()), v.x, v.y, v.z, v.w);
}


void Graphics::Shader::setMat4(const std::string & name, const Math::Mat4 & v)
{
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &v.values[0]);
}
