#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

template <class T>
class Rectangle : public Shape2<T>
{
public:
    Vec2<T> min, max;

    Rectangle() {};
    Rectangle(const Vec2<T>& p1, const Vec2<T>& p2);
    Rectangle(const Vec2<T>& p1, const T width, const T height);

    static Rectangle<T> unitRectangle();

    Rectangle(const Rectangle<T>& other) { min = other.min; max = other.max; };
    Rectangle(Rectangle<T>&& other) { min = std::move(other.min); max = std::move(other.max); };

    virtual ~Rectangle() {}

    Rectangle<T>& operator =(const Rectangle<T>& other) { min = other.min; max = other.max; return *this; };
    Rectangle<T>& operator =(Rectangle<T> && other) { min = std::move(other.min); max = std::move(other.max); return *this; };

    Rectangle<T> operator +(const Vec2<T>& v) const;
    Rectangle<T> operator -(const Vec2<T>& v) const;

    Rectangle<T>& operator +=(const Vec2<T>& v);
    Rectangle<T>& operator -=(const Vec2<T>& v);

    template <class T2>
    explicit operator Rectangle<T2>() const;

    T width() const;
    T height() const;

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

    Vec2<T> centerOfMass() const;
    T area() const;
};

typedef Rectangle<double> RectangleD;
typedef Rectangle<float> RectangleF;
typedef Rectangle<int> RectangleI;

extern template class Rectangle<double>;
extern template class Rectangle<float>;
extern template class Rectangle<int>;

#include "../src/Rectangle.cpp"
#endif // RECTANGLE_H_INCLUDED
