#pragma once

#include <ostream>
#include "include/Vec2.h"
#include "include/Vec3.h"
#include "include/Vec4.h"
#include "include/Index2.h"
#include "include/Index3.h"
#include "include/IndexN.h"
#include "include/Circle2.h"
#include "include/Rectangle2.h"
#include "include/Triangle2.h"

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Vec2<T>& v)
{
    out << '[' << v.x << ", " << v.y << ']';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Vec3<T>& v)
{
    out << '[' << v.x << ", " << v.y << ", " << v.z << ']';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Vec4<T>& v)
{
    out << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Circle2<T>& circle)
{
    out << '(' << circle.origin << ", " << circle.radius << ')';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Rectangle2<T>& rectangle)
{
    out << '(' << rectangle.min << ", " << rectangle.max << ')';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Triangle2<T>& triangle)
{
    out << '(' << triangle.vertices[0] << ", " << triangle.vertices[1] << ", " << triangle.vertices[2] << ')';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Index2Template<T>& ind)
{
    out << '(' << ind.i << ", " << ind.j << ')';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::Index3Template<T>& ind)
{
    out << '(' << ind.i << ", " << ind.j << ", " << ind.k << ')';
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const ls::IndexNTemplate<T>& ind)
{
    const int numberOfIndices = ind.indices.size();
    out << '(';
    for(int i = 0; i < numberOfIndices; ++i)
    {
        out << ind.indices[i];
        if(i != numberOfIndices - 1)
            out << ", ";
    }
    out << ')';
    return out;
}
