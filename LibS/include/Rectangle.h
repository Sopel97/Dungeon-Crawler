#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

template <class T>
class Rectangle : public Shape2<T>
{
public:
    Vec2<T> min, max;

    Rectangle() = default;
    Rectangle(const Vec2<T>& p1, const Vec2<T>& p2);
    Rectangle(const Vec2<T>& p1, const T width, const T height);

    static Rectangle<T> unitRectangle();

    Rectangle(const Rectangle<T>&) = default;
    template <class X>
    Rectangle(const Rectangle<X>& r);
    Rectangle(Rectangle<T>&&) = default;

    virtual ~Rectangle(){}

    Rectangle<T>& operator =(const Rectangle<T>&) = default;
    template <class X>
    Rectangle<T>& operator =(const Rectangle<X>& r);
    Rectangle<T>& operator =(Rectangle<T> &&) = default;

    Rectangle<T> operator +(const Vec2<T>& v) const;
    Rectangle<T> operator -(const Vec2<T>& v) const;

    Rectangle<T>& operator +=(const Vec2<T>& v);
    Rectangle<T>& operator -=(const Vec2<T>& v);

    T width() const;
    T height() const;

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    Vec2<T> center() const;

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
