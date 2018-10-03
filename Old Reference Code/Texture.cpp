#include <glad/glad.h>
#include "Texture.h"


gp::Texture::Texture(float * data, int w, int h, int nChan)
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, data);
}


gp::Texture::Texture(unsigned char *data, int w, int h, int nChan)
{
    int format = 1;
    if (nChan == 2) format = GL_RG;
    if (nChan == 3) format = GL_RGB;
    if (nChan == 4) format = GL_RGBA;
    
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}


gp::Texture::Texture(float r, float g, float b, float a)
{
    unsigned char red = (unsigned char)(255.0f * r);
    unsigned char green = (unsigned char)(255.0f * g);
    unsigned char blue = (unsigned char)(255.0f * b);
    unsigned char alpha = (unsigned char)(255.0f * a);
    
    unsigned char data[] = {red, green, blue, alpha};
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

gp::Texture::Texture(unsigned int glid) : _id(glid)
{
}


gp::Texture::Texture(Texture &&t)
{
    _id = t._id;
    t._id = 0;
}


gp::Texture::~Texture()
{
    glDeleteTextures(1, &_id);
}
