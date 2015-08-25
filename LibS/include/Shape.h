#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

template <class T>
class Shape
{
public:
    using ValueType = T;

    Shape() {}

    Shape(const Shape&) {}
    Shape(Shape&&) {}

    virtual ~Shape(){}

    Shape& operator = (const Shape&) { return *this;}
    Shape& operator = (Shape &&) { return *this;}
};

typedef Shape<double> ShapeD;
typedef Shape<float> ShapeF;
typedef Shape<int> ShapeI;

extern template class Shape<double>;
extern template class Shape<float>;
extern template class Shape<int>;

#include "../src/Shape.cpp"
#endif // SHAPE_H_INCLUDED
