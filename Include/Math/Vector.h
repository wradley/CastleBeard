#pragma once

namespace Math
{
    class Vec3;
    class Vec4;
    
    class Vec2
    {
    public:
        
        Vec2();
        Vec2(float x);
        Vec2(float x, float y);
        Vec2(const Vec2 &v);
        Vec2(const Vec3 &v);
        Vec2(const Vec4 &v);
        Vec2& operator= (const Vec2 &v);
        ~Vec2();
        
        Vec2 operator+ (const Vec2 &v) const;
        Vec2 operator- (const Vec2 &v) const;
        Vec2 operator* (float s) const;
        Vec2 operator/ (float s) const;
        
        void operator+= (const Vec2 &v);
        void operator-= (const Vec2 &v);
        void operator*= (float s);
        void operator/= (float s);
        
        float x, y;
    };
    
    
    class Vec3
    {
    public:

        Vec3();
        Vec3(float x);
        Vec3(float x, float y, float z);
        Vec3(const Vec2 &v);
        Vec3(const Vec2 &v, float z);
        Vec3(const Vec3 &v);
        Vec3(const Vec4 &v);
        Vec3& operator= (const Vec3 &v);
        ~Vec3();

        Vec3 operator+ (const Vec3 &v) const;
        Vec3 operator- (const Vec3 &v) const;
        Vec3 operator* (float s) const;
        Vec3 operator/ (float s) const;

        void operator+= (const Vec3 &v);
        void operator-= (const Vec3 &v);
        void operator*= (float s);
        void operator/= (float s);
        
        float x, y, z;
    };


    class Vec4
    {
    public:

        Vec4();
        Vec4(float x);
        Vec4(float x, float y, float z, float w);
        Vec4(const Vec2 &v);
        Vec4(const Vec2 &v, float z, float w);
        Vec4(const Vec3 &v);
        Vec4(const Vec3 &v, float w);
        Vec4(const Vec4 &v);
        Vec4& operator= (const Vec4 &v);
        ~Vec4();

        Vec4 operator+ (const Vec4 &v) const;
        Vec4 operator- (const Vec4 &v) const;
        Vec4 operator* (float s) const;
        Vec4 operator/ (float s) const;

        void operator+= (const Vec4 &v);
        void operator-= (const Vec4 &v);
        void operator*= (float s);
        void operator/= (float s);

        float x, y, z, w;
    };
}