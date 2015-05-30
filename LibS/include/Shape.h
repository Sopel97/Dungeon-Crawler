#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

template <class T>
class Shape
{
public:
    using ValueType = T;

    Shape() = default;

    Shape(const Shape&) = default;
    Shape(Shape&&) = default;

    virtual ~Shape(){}

    Shape& operator = (const Shape&) = default;
    Shape& operator = (Shape &&) = default;
};

typedef Shape<double> ShapeD;
typedef Shape<float> ShapeF;
typedef Shape<int> ShapeI;

#include "../src/Shape.cpp"
#endif // SHAPE_H_INCLUDED
