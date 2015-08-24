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
};

typedef Shape4<double> Shape4D;
typedef Shape4<float> Shape4F;
typedef Shape4<int> Shape4I;

extern template class Shape4<double>;
extern template class Shape4<float>;
extern template class Shape4<int>;

#include "../src/Shape4.cpp"
#endif // SHAPE4_H
