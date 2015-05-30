#ifndef SHAPE4_H
#define SHAPE4_H

template <class T>
class Shape4 : public Shape<T>
{
public:
    using RespectiveVectorType = Vec4<T>;

    Shape4() = default;

    Shape4(const Shape4&) = default;
    Shape4(Shape4&&) = default;

    virtual ~Shape4(){}

    Shape4& operator = (const Shape4&) = default;
    Shape4& operator = (Shape4 &&) = default;

    virtual void translate(const Vec4<T>& v) = 0;
    virtual void scale(const Vec4<T>& c, const Vec4<T>& s) = 0;
    virtual void scale(const Vec4<T>& c, const T s) = 0;
    virtual void scale(const Vec4<T>& s) = 0;
    virtual void scale(const T s) = 0;
};

typedef Shape4<double> Shape4D;
typedef Shape4<float> Shape4F;
typedef Shape4<int> Shape4I;

#include "../src/Shape4.cpp"
#endif // SHAPE4_H
