#pragma once

namespace gp
{
    class GeometryBuffer
    {
    public:
        
        GeometryBuffer(int width, int height);
        ~GeometryBuffer();
        
        void bindRead();
        void bindWrite();
        void bindAll();
		inline unsigned int getID() { return _id; }
        
        inline unsigned int getPositionID() const { return _position; }
        inline unsigned int getNormalID() const { return _normal; }
        inline unsigned int getColorID() const { return _color; }
        inline unsigned int getMRAID() const { return _mra; }
        
        void clear();
        void resize(int width, int height);
        
    private:
        
        void free();
        
    private:
        
        unsigned int _position, _color, _normal, _mra, _depth;
        int _width, _height;
        unsigned int _id;
        
    };
    
    
    
    class LightBuffer
    {
    public:
        
        LightBuffer(int width, int height);
        ~LightBuffer();
        
        void bindRead();
        void bindWrite();
        void bindAll();
		inline unsigned int getID() { return _id; }
        
        inline unsigned int getColorID() const { return _color; }
        
        void clear();
        void resize(int width, int height);
        
    private:
        
        void free();
        
    private:
        
        unsigned int _color, _depth;
        int _width, _height;
        unsigned int _id;
        
    };
}











