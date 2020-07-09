#ifndef _Vector3f_H_
#define _Vector3f_H_

#include <cmath>

class Vector3f
{
public:
    union {
        float x;
        float r;
    };
    union {
        float y;
        float g;
    };
    union {
        float z;
        float b;
    };

    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

    bool IsEqual(const Vector3f &f) const
    {
        if (x == f.x && y == f.y && z == f.z)
        {
            return true;
        }
        return false;
    }

    Vector3f operator*(float f) const
    {
        return Vector3f(x * f, y * f, z * f);
    }
    Vector3f operator*(const Vector3f &f) const
    {
        return Vector3f(x * f.x, y * f.y, z * f.z);
    }

    Vector3f &operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    Vector3f &operator*=(const Vector3f &f)
    {
        x *= f.x;
        y *= f.y;
        z *= f.z;
        return *this;
    }

    Vector3f operator/(float f) const
    {
        return Vector3f(x / f, y / f, z / f);
    }

    Vector3f &operator/=(float f)
    {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    Vector3f operator+(const Vector3f &f) const
    {
        return Vector3f(x + f.x, y + f.y, z + f.z);
    }

    Vector3f &operator+=(const Vector3f &f)
    {
        x += f.x;
        y += f.y;
        z += f.z;
        return *this;
    }

    Vector3f operator-(const Vector3f &f) const
    {
        return Vector3f(x - f.x, y - f.y, z - f.z);
    }

    Vector3f &operator-=(const Vector3f &f)
    {
        x -= f.x;
        y -= f.y;
        z -= f.z;
        return *this;
    }

    float Dot(const Vector3f &f) const
    {
        return (x * f.x) + (y * f.y) + (z * f.z);
    }

    Vector3f Cross(const Vector3f &f) const
    {
        return Vector3f(
            y * f.z - z * f.y,
            z * f.x - x * f.z,
            x * f.y - y * f.x);
    }

    Vector3f Normalize() const
    {
        float length = this->Dot(*this);
        if (length == 0)
            return *this;

        return *this / sqrt(length);
    }

private:
};

#endif