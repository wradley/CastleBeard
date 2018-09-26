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
        Vec2(float (&x)[2]);
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
        
        Vec2& operator+= (const Vec2 &v);
        Vec2& operator-= (const Vec2 &v);
        Vec2& operator*= (float s);
        Vec2& operator/= (float s);
        
        float x, y;
    };
    
    
    class Vec3
    {
    public:

        Vec3();
        Vec3(float x);
        Vec3(float(&x)[3]);
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

        Vec3& operator+= (const Vec3 &v);
        Vec3& operator-= (const Vec3 &v);
        Vec3& operator*= (float s);
        Vec3& operator/= (float s);
        
        float x, y, z;
    };


    class Vec4
    {
    public:

        Vec4();
        Vec4(float x);
        Vec4(float(&x)[4]);
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

        Vec4& operator+= (const Vec4 &v);
        Vec4& operator-= (const Vec4 &v);
        Vec4& operator*= (float s);
        Vec4& operator/= (float s);

        float x, y, z, w;
    };

    float Length(const Vec2 &v);
    float Length(const Vec3 &v);
    float Length(const Vec4 &v);

    Vec2 Normalize(const Vec2 &v);
    Vec3 Normalize(const Vec3 &v);
    Vec4 Normalize(const Vec4 &v);

    float Dot(const Vec2 &a, const Vec2 &b);
    float Dot(const Vec3 &a, const Vec3 &b);
    float Dot(const Vec4 &a, const Vec4 &b);

    Vec3 Cross(const Vec3 &a, const Vec3 &b);
}