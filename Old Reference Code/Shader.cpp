#include <glad/glad.h>
#include "Shader.h"


gp::Shader::Shader(
    const std::string &vCd, std::string &vErrRet,
    const std::string &fCd, std::string &fErrRet,
    const std::string &gCd, std::string &gErrRet,
    std::string &linkErrRet
) : _valid(true) 
{
    const char *vCode = vCd.c_str();
    const char *fCode = fCd.c_str();
    const char *gCode = gCd.c_str();
    
    
    const int BUFF_SZ = 1024;
    int success = 0;
    char buff[BUFF_SZ];
    
    // vertex
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vCode, 0);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader, BUFF_SZ, NULL, buff);
        vErrRet = buff;
		_valid = false;
    }
    
    // frag
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fCode, 0);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, BUFF_SZ, NULL, buff);
        fErrRet = buff;
		_valid = false;
    }
    
    // geometry
    unsigned int gShader = 0;
    if (gCd.size()) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &gCode, 0);
        glCompileShader(gShader);
        glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(gShader, BUFF_SZ, NULL, buff);
            gErrRet = buff;
			_valid = false;
        }
    }
    
    // link
    _id = glCreateProgram();
    glAttachShader(_id, vShader);
    glAttachShader(_id, fShader);
    if (gCd.size()) glAttachShader(_id, gShader);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, BUFF_SZ, NULL, buff);
        linkErrRet = buff;
		_valid = false;
    }
    
    if (vErrRet.size() == 0) glDeleteShader(vShader);
    if (fErrRet.size() == 0) glDeleteShader(fShader);
    if (gErrRet.size() == 0) glDeleteShader(gShader);
}


gp::Shader::Shader(Shader &&s)
{
    _id = s._id;
	_valid = true;
    s._id = 0;
	s._valid = false;
}


gp::Shader::Shader(unsigned int glid) : _id(glid), _valid(true)
{}


gp::Shader::~Shader()
{
	if (_valid) glDeleteProgram(_id);
}


void gp::Shader::bind()
{
	if (_valid) glUseProgram(_id);
}


void gp::Shader::setBool(const std::string &name, bool v)
{
	if (_valid) glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)v);
}


void gp::Shader::setInt(const std::string &name, int v)
{
	if (_valid) glUniform1i(glGetUniformLocation(_id, name.c_str()), v);
}


void gp::Shader::setFloat(const std::string &name, float v)
{
	if (_valid) glUniform1f(glGetUniformLocation(_id, name.c_str()), v);
}


void gp::Shader::setVec2(const std::string &name, const glm::vec2 &v)
{
	if (_valid) glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &v[0]);
}


void gp::Shader::setVec3(const std::string &name, const glm::vec3 &v)
{
	if (_valid) glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &v[0]);
}


void gp::Shader::setVec4(const std::string &name, const glm::vec4 &v)
{
	if (_valid) glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &v[0]);
}


void gp::Shader::setMat2(const std::string &name, const glm::mat2 &v)
{
	if (_valid) glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &v[0][0]);
}


void gp::Shader::setMat3(const std::string &name, const glm::mat3 &v)
{
	if (_valid) glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &v[0][0]);
}


void gp::Shader::setMat4(const std::string &name, const glm::mat4 &v)
{
	if (_valid) glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &v[0][0]);
}


void gp::Shader::setTexture(
    const std::string &name,
    unsigned int n,
    const Texture &t
){
	if (_valid) {
		glActiveTexture(GL_TEXTURE0 + n);
		glBindTexture(GL_TEXTURE_2D, t._id);
		setInt(name, n);
	}
}


void gp::Shader::setTexture(
    const std::string &name,
    unsigned int n,
    unsigned int glID
){
	if (_valid) {
		glActiveTexture(GL_TEXTURE0 + n);
		glBindTexture(GL_TEXTURE_2D, glID);
		setInt(name, n);
	}
}


void gp::Shader::setCubeMap(const std::string & name, unsigned int n, const CubeMap & c)
{
	if (_valid) {
		glActiveTexture(GL_TEXTURE0 + n);
		glBindTexture(GL_TEXTURE_CUBE_MAP, c._id);
		setInt(name, n);
	}
}