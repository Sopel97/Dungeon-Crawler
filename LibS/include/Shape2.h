#ifndef Shape2_H_INCLUDED
#define Shape2_H_INCLUDED

/*  Available 2D shapes

    Circle
    LineSegment
    Polygon
    Polyline
    Ray
    Rectangle
    Triangle
    Vec2

*/

template <class T>
class Shape2 : public Shape<T>
{
public:
    struct RandomPointPickerPreprocessedData //Stores data for pickRandomPoint function. Sending it through parameter will save some time (depending on shape) for preprocessing. Every shape has it's own struct deriving from this one.
    {

    };

    using VectorType = Vec2<T>;

    Shape2() {}

    Shape2(const Shape2&) {}
    Shape2(Shape2&&) {}
    Shape2<T>& operator=(const Shape2&) { return *this; }
    Shape2<T>& operator=(Shape2&&) { return *this; }

    virtual ~Shape2() {}
};

typedef Shape2<double> Shape2D;
typedef Shape2<float> Shape2F;
typedef Shape2<int> Shape2I;

extern template class Shape2<double>;
extern template class Shape2<float>;
extern template class Shape2<int>;

#include "../src/Shape2.cpp"
#endif // Shape2_H_INCLUDED
