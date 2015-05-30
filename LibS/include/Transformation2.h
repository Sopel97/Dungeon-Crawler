#ifndef TRANSFORMATION2_H
#define TRANSFORMATION2_H

template <class T>
class Transformation2
{
public:
    Transformation2() = default;

    virtual void transform(Vec2<T>& point) const = 0;
    virtual Vec2<T> transformed(const Vec2<T>& point) const = 0;
protected:
};

typedef Transformation2<double> Transformation2D;
typedef Transformation2<float> Transformation2F;
typedef Transformation2<int> Transformation2I;

#include "../src/Transformation2.cpp"

#endif // TRANSFORMATION2_H
