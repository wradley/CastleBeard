#pragma once
#include <memory>

namespace gp
{
    class Texture
    {
    public:
        
		Texture(float *data, int w, int h, int nChan);
        Texture(unsigned char *data, int w, int h, int nChan);
        Texture(float r, float g, float b, float a = 1.0f);
		Texture(unsigned int glid);
        Texture(Texture &&t);
        ~Texture();
        
    private:
        
        friend class Shader;
        unsigned int _id;
        
    private:
        
        Texture(const Texture &t) = delete;
        Texture& operator= (const Texture &t) = delete;
        
    };
}
