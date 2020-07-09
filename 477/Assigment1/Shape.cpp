#include "Shape.h"
#include "Scene.h"
#include <cstdio>
#include <limits>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{
}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
    center = (*pVertices)[cIndex - 1];
    radius = R;
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray &ray) const
{
    ReturnVal retVal;
    retVal.matIndex = -1;

    Vector3f originCenter = ray.origin - center;
    Vector3f d = ray.direction;

    float a = d.Dot(d);
    float b = d.Dot(originCenter);
    float c = originCenter.Dot(originCenter) - radius * radius;

    float quad = b * b - a * c;
    float test = pScene->intTestEps;
    float sqrt = std::sqrt(quad);

    if (quad <= test)
    {
        return retVal;
    }

    float root1 = (-b + sqrt) / a;
    float root2 = (-b - sqrt) / a;

    if (root1 <= test)
    {
        return retVal;
    }

    if (root2 < root1 && root2 > test)
    {
        retVal.t = root2;
    }
    else
    {
        retVal.t = root1;
    }

    retVal.point = ray.getPoint(retVal.t);
    retVal.matIndex = matIndex;
    retVal.normal = (retVal.point - center).Normalize();
    return retVal;
}

Triangle::Triangle(void)
{
}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
    a = (*pVertices)[p1Index - 1];
    b = (*pVertices)[p2Index - 1];
    c = (*pVertices)[p3Index - 1];

    ba = b - a;
    ca = c - a;
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray &ray) const
{
    ReturnVal ret;
    ret.matIndex = -1;
    Vector3f direction = ray.direction;
    Vector3f n = ba.Cross(ca).Normalize();
    Vector3f q = direction.Cross(ca);

    float a1 = ba.Dot(q);
    float test = pScene->intTestEps;

    if ((n.Dot(direction) >= 0) || a1 <= test)
        return ret;

    Vector3f s = (ray.origin - a);
    float b0 = s.Dot(q) / a1;

    if (b0 < 0 || b0 > 1)
        return ret;

    Vector3f r = s.Cross(ba);
    float b1 = r.Dot(direction) / a1;
    float b2 = 1 - b0 - b1;

    if (b1 < 0 || b2 < 0)
        return ret;

    float t = ca.Dot(r) / a1;

    if (t < 0)
        return ret;

    ret.matIndex = matIndex;
    ret.t = t;
    ret.point = ray.getPoint(t);
    ret.normal = n;

    return ret;
}

Mesh::Mesh()
{
}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle> &faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex), faces(faces)
{
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray &ray) const
{
    ReturnVal retVal, inter;
    retVal.matIndex = -1;

    float t = std::numeric_limits<float>::infinity();

    for (int f = 0; f < faces.size(); f++)
    {
        inter = faces[f].intersect(ray);
        if (inter.matIndex != -1)
        {
            if (inter.t > 0 && inter.t < t)
            {
                retVal = inter;
                t = inter.t;
            }
        }
    }

    return retVal;
}
