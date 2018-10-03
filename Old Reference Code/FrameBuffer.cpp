#include <glad/glad.h>
#include <iostream>
#include "FrameBuffer.h"

gp::GeometryBuffer::GeometryBuffer(int width, int height) :
    _position(0), _color(0), _normal(0), _mra(0), _depth(0),
    _width(0), _height(0), _id(0)
{
    resize(width, height);
}


gp::GeometryBuffer::~GeometryBuffer()
{
    free();
}


void gp::GeometryBuffer::bindRead()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
}


void gp::GeometryBuffer::bindWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}


void gp::GeometryBuffer::bindAll()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}


void gp::GeometryBuffer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void gp::GeometryBuffer::resize(int w, int h)
{
    if (w == _width && h == _height) return;
    free();
    _width = w;
    _height = h;
    
    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    
    // position
    glGenTextures(1, &_position);
    glBindTexture(GL_TEXTURE_2D, _position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _position, 0);
    
    // normal
    glGenTextures(1, &_normal);
    glBindTexture(GL_TEXTURE_2D, _normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _normal, 0);
    
    // color
    glGenTextures(1, &_color);
    glBindTexture(GL_TEXTURE_2D, _color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _color, 0);
    
    // mra
    glGenTextures(1, &_mra);
    glBindTexture(GL_TEXTURE_2D, _mra);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _mra, 0);
    
    unsigned int attachments[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3
    };
    glDrawBuffers(4, attachments);
    
    // depth buffer
    glGenRenderbuffers(1, &_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, _depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
    
    // error check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Geometry buffer not complete" << std::endl;
    
    glViewport(0, 0, w, h);
}


void gp::GeometryBuffer::free()
{
    glDeleteTextures(1, &_position);
    glDeleteTextures(1, &_normal);
    glDeleteTextures(1, &_color);
    glDeleteTextures(1, &_mra);
    glDeleteRenderbuffers(1, &_depth);
    glDeleteFramebuffers(1, &_id);
}


gp::LightBuffer::LightBuffer(int width, int height) :
    _color(0), _depth(0), _width(0), _height(0), _id(0)
{
    resize(width, height);
}


gp::LightBuffer::~LightBuffer()
{
    free();
}


void gp::LightBuffer::bindRead()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
}


void gp::LightBuffer::bindWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}


void gp::LightBuffer::bindAll()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}


void gp::LightBuffer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}


void gp::LightBuffer::resize(int w, int h)
{
    if (w == _width && h == _height) return;
    free();
    _width = w;
    _height = h;
    
    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    
    // color
    glGenTextures(1, &_color);
    glBindTexture(GL_TEXTURE_2D, _color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _color, 0);
    
    unsigned int attachments[] = {
        GL_COLOR_ATTACHMENT0
    };
    glDrawBuffers(1, attachments);
    
    // depth buffer
    glGenRenderbuffers(1, &_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, _depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
    
    // error check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Lighting buffer not complete" << std::endl;
    
    glViewport(0, 0, w, h);
}


void gp::LightBuffer::free()
{
    glDeleteTextures(1, &_color);
    glDeleteRenderbuffers(1, &_depth);
    glDeleteFramebuffers(1, &_id);
}





























