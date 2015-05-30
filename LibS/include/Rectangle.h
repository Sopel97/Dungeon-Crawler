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

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Rectangle<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Rectangle<T> transformed(const Transformation2<T>& transformation) const;

    virtual T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual Vec2<T> center() const;
    virtual T signedArea() const;

#ifndef GEOMETRY_LIGHT
    virtual bool intersects(const Shape2<T>* other) const {return other->intersects(*this);}
    virtual bool contains(const Shape2<T>* other) const {return other->isContained(*this);}
    virtual bool isContained(const Shape2<T>* other) const {return other->contains(*this);}
#endif // GEOMETRY_LIGHT
    SHAPE2_DOUBLE_DISPATCHING_METHODS

    virtual std::unique_ptr<Shape2<T>> clone() const;
};

typedef Rectangle<double> RectangleD;
typedef Rectangle<float> RectangleF;
typedef Rectangle<int> RectangleI;

#include "../src/Rectangle.cpp"
#endif // RECTANGLE_H_INCLUDED
