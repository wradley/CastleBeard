#include <cmath>
#include "../Include/Math/Matrix.h"
#include "../Include/Math/Vector.h"


Math::Mat3 Math::RotateEuler(const Mat3 & m, float x, float y, float z)
{
    Mat3 rx({
        1.0f,    0.0f,   0.0f,
        0.0f,  cos(x), sin(x),
        0.0f, -sin(x), cos(x)
    });

    Mat3 ry({
        cos(y), 0.0f, -sin(y),
          0.0f, 1.0f,    0.0f,
        sin(y), 0.0f,   cos(y)
    });

    Mat3 rz({
         cos(z), sin(z), 0.0f,
        -sin(z), cos(z), 0.0f,
         0.0f,     0.0f, 1.0f
    });

    return m * rz * ry * rz;
}


Math::Mat4 Math::RotateEuler(const Mat4 & m, float x, float y, float z)
{
    Mat4 rot(RotateEuler(Mat3(1.0f), x, y, z));
    rot.values[15] = 1.0f;
    return m * rot;
}


Math::Mat4 Math::Transpose(const Mat4 & m, const Vec3 & v)
{
    Mat4 ret(m);
    ret.values[3] += v.x;
    ret.values[7] += v.y;
    ret.values[11] += v.z;
    return ret;
}


// ============================================================================


Math::Mat2::Mat2() : 
    values{
        0.0f, 0.0f, 
        0.0f, 0.0f  }
{}


Math::Mat2::Mat2(float i) :
    values{
           i, 0.0f,
        0.0f,    i  }
{}


Math::Mat2::Mat2(const float(&i)[4]) : values{
    /*i[0], i[2],
    i[1], i[3]*/
    i[0], i[1],
    i[2], i[3]
}{}


Math::Mat2::Mat2(const Mat2 & m) : values{ 
    m.values[0], m.values[1], 
    m.values[2], m.values[3] 
}{}


Math::Mat2::Mat2(const Mat3 & m) : values{ 
    m.values[0], m.values[1],
    m.values[3], m.values[4] 
}{}


Math::Mat2::Mat2(const Mat4 & m) : values{ 
    m.values[0], m.values[1],
    m.values[4], m.values[5] 
}{}


Math::Mat2& Math::Mat2::operator=(const Mat2 & m)
{
    values[0] = m.values[0];
    values[1] = m.values[1];
    values[2] = m.values[2];
    values[3] = m.values[3];
    return *this;
}


Math::Mat2 Math::Mat2::operator*(float s) const
{
    Mat2 r;
    r.values[0] = values[0] * s;
    r.values[1] = values[1] * s;
    r.values[2] = values[2] * s;
    r.values[3] = values[3] * s;
    return r;
}


Math::Vec2 Math::Mat2::operator*(const Vec2 & v) const
{
    float x, y;
    /*x = (values[0] * v.x) + (values[2] * v.y);
    y = (values[1] * v.x) + (values[3] * v.y);*/
    x = (values[0] * v.x) + (values[1] * v.y);
    y = (values[2] * v.x) + (values[3] * v.y);
    return Vec2(x, y);
}


Math::Mat2 Math::Mat2::operator*(const Mat2 & v) const
{
    auto &othr = v.values;
    auto &me = values;

    Mat2 r;
    /*r.values[0] = (me[0] * othr[0]) + (me[2] * othr[1]);
    r.values[1] = (me[1] * othr[0]) + (me[3] * othr[1]);
    r.values[2] = (me[0] * othr[2]) + (me[2] * othr[3]);
    r.values[3] = (me[1] * othr[2]) + (me[3] * othr[3]);*/
    r.values[0] = (me[0] * othr[0]) + (me[1] * othr[2]);
    r.values[2] = (me[2] * othr[0]) + (me[3] * othr[2]);
    r.values[1] = (me[0] * othr[1]) + (me[1] * othr[3]);
    r.values[3] = (me[2] * othr[1]) + (me[3] * othr[3]);
    return r;
}


// ============================================================================


Math::Mat3::Mat3() : values{
    0.0f, 0.0f , 0.0f,
    0.0f, 0.0f , 0.0f,
    0.0f, 0.0f , 0.0f,
}{}


Math::Mat3::Mat3(float i) : values{
    i   , 0.0f , 0.0f,
    0.0f, i    , 0.0f,
    0.0f, 0.0f , i
} {}


Math::Mat3::Mat3(const float(&i)[9]) : values{
    /*i[0], i[3], i[6],
    i[1], i[4], i[7],
    i[2], i[5], i[8]*/
    i[0], i[1], i[2],
    i[3], i[4], i[5],
    i[6], i[7], i[8]
} {}


Math::Mat3::Mat3(const Mat2 & m) : values{
    m.values[0], m.values[1], 0.0f,
    m.values[2], m.values[3], 0.0f,
    0.0f, 0.0f, 0.0f,
} {}

Math::Mat3::Mat3(const Mat3 & m) : values{
    m.values[0], m.values[1], m.values[2],
    m.values[4], m.values[5], m.values[6],
    m.values[7], m.values[8], m.values[10]
}{}


Math::Mat3::Mat3(const Mat4 & m) : values{
    m.values[0], m.values[1], m.values[2],
    m.values[3], m.values[4], m.values[5],
    m.values[6], m.values[7], m.values[8]
} {}


Math::Mat3 & Math::Mat3::operator=(const Mat3 & m)
{
    values[0] = m.values[0];
    values[1] = m.values[1];
    values[2] = m.values[2];
    values[3] = m.values[3];
    values[4] = m.values[4];
    values[5] = m.values[5];
    values[6] = m.values[6];
    values[7] = m.values[7];
    values[8] = m.values[8];
    return *this;
}


Math::Mat3 Math::Mat3::operator*(float s) const
{
    Mat3 ret;
    ret.values[0] = values[0] * s;
    ret.values[1] = values[1] * s;
    ret.values[2] = values[2] * s;
    ret.values[3] = values[3] * s;
    ret.values[4] = values[4] * s;
    ret.values[5] = values[5] * s;
    ret.values[6] = values[6] * s;
    ret.values[7] = values[7] * s;
    ret.values[8] = values[8] * s;
    return ret;
}


Math::Vec3 Math::Mat3::operator*(const Vec3 & v) const
{
    Vec3 ret;
    /*ret.x = (values[0] * v.x) + (values[3] * v.y) + (values[6] * v.z);
    ret.y = (values[1] * v.x) + (values[4] * v.y) + (values[7] * v.z);
    ret.z = (values[2] * v.x) + (values[5] * v.y) + (values[8] * v.z);*/
    ret.x = (values[0] * v.x) + (values[1] * v.y) + (values[2] * v.z);
    ret.y = (values[3] * v.x) + (values[4] * v.y) + (values[5] * v.z);
    ret.z = (values[6] * v.x) + (values[7] * v.y) + (values[8] * v.z);
    return ret;
}


Math::Mat3 Math::Mat3::operator*(const Mat3 & m) const
{
    Mat3 ret;

    auto &me = values;
    auto &othr = m.values;
    
    /*ret.values[0] = (me[0] * othr[0]) + (me[3] * othr[1]) + (me[6] * othr[2]);
    ret.values[1] = (me[1] * othr[0]) + (me[4] * othr[1]) + (me[7] * othr[2]);
    ret.values[2] = (me[2] * othr[0]) + (me[5] * othr[1]) + (me[8] * othr[2]);
    ret.values[3] = (me[0] * othr[3]) + (me[3] * othr[4]) + (me[6] * othr[5]);
    ret.values[4] = (me[1] * othr[3]) + (me[4] * othr[4]) + (me[7] * othr[5]);
    ret.values[5] = (me[2] * othr[3]) + (me[5] * othr[4]) + (me[8] * othr[5]);
    ret.values[6] = (me[0] * othr[6]) + (me[3] * othr[7]) + (me[6] * othr[8]);
    ret.values[7] = (me[1] * othr[6]) + (me[4] * othr[7]) + (me[7] * othr[8]);
    ret.values[8] = (me[2] * othr[6]) + (me[5] * othr[7]) + (me[8] * othr[8]);*/
    ret.values[0] = (me[0] * othr[0]) + (me[1] * othr[3]) + (me[2] * othr[6]);
    ret.values[3] = (me[3] * othr[0]) + (me[4] * othr[3]) + (me[5] * othr[6]);
    ret.values[6] = (me[6] * othr[0]) + (me[7] * othr[3]) + (me[8] * othr[6]);
    ret.values[1] = (me[0] * othr[1]) + (me[1] * othr[4]) + (me[2] * othr[7]);
    ret.values[4] = (me[3] * othr[1]) + (me[4] * othr[4]) + (me[5] * othr[7]);
    ret.values[7] = (me[6] * othr[1]) + (me[7] * othr[4]) + (me[8] * othr[7]);
    ret.values[2] = (me[0] * othr[2]) + (me[1] * othr[5]) + (me[2] * othr[8]);
    ret.values[5] = (me[3] * othr[2]) + (me[4] * othr[5]) + (me[5] * othr[8]);
    ret.values[8] = (me[6] * othr[2]) + (me[7] * othr[5]) + (me[8] * othr[8]);

    return ret;
}


// ============================================================================


Math::Mat4::Mat4() : values{
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
} {}


Math::Mat4::Mat4(float i) : values{
       i, 0.0f, 0.0f, 0.0f,
    0.0f,    i, 0.0f, 0.0f,
    0.0f, 0.0f,    i, 0.0f,
    0.0f, 0.0f, 0.0f, i
} {}


Math::Mat4::Mat4(const float(&i)[16]) : values{
    /*i[0], i[4], i[ 8], i[12],
    i[1], i[5], i[ 9], i[13],
    i[2], i[6], i[10], i[14],
    i[3], i[7], i[11], i[15]*/
    i[ 0], i[ 1], i[ 2], i[ 3],
    i[ 4], i[ 5], i[ 6], i[ 7],
    i[ 8], i[ 9], i[10], i[11],
    i[12], i[13], i[14], i[15]
} {}


Math::Mat4::Mat4(const Mat2 & m) : values{
    m.values[0], m.values[1], 0.0f, 0.0f,
    m.values[2], m.values[3], 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
} {}


Math::Mat4::Mat4(const Mat3 & m) : values{
    m.values[0], m.values[1], m.values[2], 0.0f,
    m.values[3], m.values[4], m.values[5], 0.0f,
    m.values[6], m.values[7], m.values[8], 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
} {}

Math::Mat4::Mat4(const Mat4 & m) : values{
    m.values[ 0], m.values[ 1], m.values[ 2], m.values[ 3],
    m.values[ 4], m.values[ 5], m.values[ 6], m.values[ 7],
    m.values[ 8], m.values[ 9], m.values[10], m.values[11],
    m.values[12], m.values[13], m.values[14], m.values[15]
} {}


Math::Mat4 & Math::Mat4::operator=(const Mat4 & m)
{
    values[ 0] = m.values[ 0];
    values[ 1] = m.values[ 1];
    values[ 2] = m.values[ 2];
    values[ 3] = m.values[ 3];
    values[ 4] = m.values[ 4];
    values[ 5] = m.values[ 5];
    values[ 6] = m.values[ 6];
    values[ 7] = m.values[ 7];
    values[ 8] = m.values[ 8];
    values[ 9] = m.values[ 9];
    values[10] = m.values[10];
    values[11] = m.values[11];
    values[12] = m.values[12];
    values[13] = m.values[13];
    values[14] = m.values[14];
    values[15] = m.values[15];
    return *this;
}


Math::Mat4 Math::Mat4::operator*(float s) const
{
    Mat4 ret;
    ret.values[ 0] = values[ 0] * s;
    ret.values[ 1] = values[ 1] * s;
    ret.values[ 2] = values[ 2] * s;
    ret.values[ 3] = values[ 3] * s;
    ret.values[ 4] = values[ 4] * s;
    ret.values[ 5] = values[ 5] * s;
    ret.values[ 6] = values[ 6] * s;
    ret.values[ 7] = values[ 7] * s;
    ret.values[ 8] = values[ 8] * s;
    ret.values[ 9] = values[ 9] * s;
    ret.values[10] = values[10] * s;
    ret.values[11] = values[11] * s;
    ret.values[12] = values[12] * s;
    ret.values[13] = values[13] * s;
    ret.values[14] = values[14] * s;
    ret.values[15] = values[15] * s;
    return ret;
}


Math::Vec4 Math::Mat4::operator*(const Vec4 & v) const
{
    Vec4 ret;
    /*ret.x = (values[ 0] * v.x) + (values[ 4] * v.y) + (values[ 8] * v.z) + (values[12] * v.w);
    ret.y = (values[ 1] * v.x) + (values[ 5] * v.y) + (values[ 9] * v.z) + (values[13] * v.w);
    ret.z = (values[ 2] * v.x) + (values[ 6] * v.y) + (values[10] * v.z) + (values[14] * v.w);
    ret.w = (values[ 3] * v.x) + (values[ 7] * v.y) + (values[11] * v.z) + (values[15] * v.w);*/
    ret.x = (values[ 0] * v.x) + (values[ 1] * v.y) + (values[ 2] * v.z) + (values[ 3] * v.w);
    ret.y = (values[ 4] * v.x) + (values[ 5] * v.y) + (values[ 6] * v.z) + (values[ 7] * v.w);
    ret.z = (values[ 8] * v.x) + (values[ 9] * v.y) + (values[10] * v.z) + (values[11] * v.w);
    ret.w = (values[12] * v.x) + (values[13] * v.y) + (values[14] * v.z) + (values[15] * v.w);
    return ret;
}


Math::Mat4 Math::Mat4::operator*(const Mat4 & m) const
{
    auto &me = values;
    auto &othr = m.values;

    Mat4 ret;

    /*ret.values[ 0] = (me[ 0] * othr[0]) + (me[ 4] * othr[1]) + (me[ 8] * othr[2]) + (me[12] * othr[3]);
    ret.values[ 1] = (me[ 1] * othr[0]) + (me[ 5] * othr[1]) + (me[ 9] * othr[2]) + (me[13] * othr[3]);
    ret.values[ 2] = (me[ 2] * othr[0]) + (me[ 6] * othr[1]) + (me[10] * othr[2]) + (me[14] * othr[3]);
    ret.values[ 3] = (me[ 3] * othr[0]) + (me[ 7] * othr[1]) + (me[11] * othr[2]) + (me[15] * othr[3]);

    ret.values[ 4] = (me[0] * othr[4]) + (me[4] * othr[5]) + (me[ 8] * othr[6]) + (me[12] * othr[7]);
    ret.values[ 5] = (me[1] * othr[4]) + (me[5] * othr[5]) + (me[ 9] * othr[6]) + (me[13] * othr[7]);
    ret.values[ 6] = (me[2] * othr[4]) + (me[6] * othr[5]) + (me[10] * othr[6]) + (me[14] * othr[7]);
    ret.values[ 7] = (me[3] * othr[4]) + (me[7] * othr[5]) + (me[11] * othr[6]) + (me[15] * othr[7]);

    ret.values[ 8] = (me[0] * othr[8]) + (me[4] * othr[9]) + (me[ 8] * othr[10]) + (me[12] * othr[11]);
    ret.values[ 9] = (me[1] * othr[8]) + (me[5] * othr[9]) + (me[ 9] * othr[10]) + (me[13] * othr[11]);
    ret.values[10] = (me[2] * othr[8]) + (me[6] * othr[9]) + (me[10] * othr[10]) + (me[14] * othr[11]);
    ret.values[11] = (me[3] * othr[8]) + (me[7] * othr[9]) + (me[11] * othr[10]) + (me[15] * othr[11]);

    ret.values[12] = (me[0] * othr[12]) + (me[4] * othr[13]) + (me[ 8] * othr[14]) + (me[12] * othr[15]);
    ret.values[13] = (me[1] * othr[12]) + (me[5] * othr[13]) + (me[ 9] * othr[14]) + (me[13] * othr[15]);
    ret.values[14] = (me[2] * othr[12]) + (me[6] * othr[13]) + (me[10] * othr[14]) + (me[14] * othr[15]);
    ret.values[15] = (me[3] * othr[12]) + (me[7] * othr[13]) + (me[11] * othr[14]) + (me[15] * othr[15]);*/

    ret.values[ 0] = (me[ 0] * othr[0]) + (me[ 1] * othr[4]) + (me[ 2] * othr[ 8]) + (me[ 3] * othr[12]);
    ret.values[ 4] = (me[ 4] * othr[0]) + (me[ 5] * othr[4]) + (me[ 6] * othr[ 8]) + (me[ 7] * othr[12]);
    ret.values[ 8] = (me[ 8] * othr[0]) + (me[ 9] * othr[4]) + (me[10] * othr[ 8]) + (me[11] * othr[12]);
    ret.values[12] = (me[12] * othr[0]) + (me[13] * othr[4]) + (me[14] * othr[ 8]) + (me[15] * othr[12]);

    ret.values[ 1] = (me[ 0] * othr[1]) + (me[ 1] * othr[5]) + (me[ 2] * othr[ 9]) + (me[ 3] * othr[13]);
    ret.values[ 5] = (me[ 4] * othr[1]) + (me[ 5] * othr[5]) + (me[ 6] * othr[ 9]) + (me[ 7] * othr[13]);
    ret.values[ 9] = (me[ 8] * othr[1]) + (me[ 9] * othr[5]) + (me[10] * othr[ 9]) + (me[11] * othr[13]);
    ret.values[13] = (me[12] * othr[1]) + (me[13] * othr[5]) + (me[14] * othr[ 9]) + (me[15] * othr[13]);

    ret.values[ 2] = (me[ 0] * othr[2]) + (me[ 1] * othr[6]) + (me[ 2] * othr[10]) + (me[ 3] * othr[14]);
    ret.values[ 6] = (me[ 4] * othr[2]) + (me[ 5] * othr[6]) + (me[ 6] * othr[10]) + (me[ 7] * othr[14]);
    ret.values[10] = (me[ 8] * othr[2]) + (me[ 9] * othr[6]) + (me[10] * othr[10]) + (me[11] * othr[14]);
    ret.values[14] = (me[12] * othr[2]) + (me[13] * othr[6]) + (me[14] * othr[10]) + (me[15] * othr[14]);

    ret.values[ 3] = (me[ 0] * othr[3]) + (me[ 1] * othr[7]) + (me[ 2] * othr[11]) + (me[ 3] * othr[15]);
    ret.values[ 7] = (me[ 4] * othr[3]) + (me[ 5] * othr[7]) + (me[ 6] * othr[11]) + (me[ 7] * othr[15]);
    ret.values[11] = (me[ 8] * othr[3]) + (me[ 9] * othr[7]) + (me[10] * othr[11]) + (me[11] * othr[15]);
    ret.values[15] = (me[12] * othr[3]) + (me[13] * othr[7]) + (me[14] * othr[11]) + (me[15] * othr[15]);

    return ret;
}



