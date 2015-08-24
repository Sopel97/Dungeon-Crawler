#ifndef SHAPE3_H
#define SHAPE3_H

template <class T>
class Shape3 : public Shape<T>
{
public:
    using RespectiveVectorType = Vec3<T>;

    Shape3() = default;

    Shape3(const Shape3&) = default;
    Shape3(Shape3&&) = default;

    virtual ~Shape3(){}

    Shape3& operator = (const Shape3&) = default;
    Shape3& operator = (Shape3 &&) = default;
};

typedef Shape3<double> Shape3D;
typedef Shape3<float> Shape3F;
typedef Shape3<int> Shape3I;

extern template class Shape3<double>;
extern template class Shape3<float>;
extern template class Shape3<int>;

#include "../src/Shape3.cpp"
#endif // SHAPE3_H
