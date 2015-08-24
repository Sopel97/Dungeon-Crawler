#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

template <class T>
class Circle : public Shape2<T>
{
public:
    Vec2<T> origin;
    T radius;

    Circle() = default;
    Circle(const Vec2<T>& p1, T r);

    Circle(const Circle<T>&) = default;
    Circle(Circle<T>&&) = default;

    virtual ~Circle(){}

    Circle<T>& operator =(Circle<T> &&) = default;
    Circle<T>& operator =(const Circle<T>&) = default;

    Circle<T> operator +(const Vec2<T>& v) const;
    Circle<T> operator -(const Vec2<T>& v) const;

    Circle<T>& operator +=(const Vec2<T>& v);
    Circle<T>& operator -=(const Vec2<T>& v);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    Vec2<T> center() const;
};

typedef Circle<double> CircleD;
typedef Circle<float> CircleF;
typedef Circle<int> CircleI;

extern template class Circle<double>;
extern template class Circle<float>;
extern template class Circle<int>;

#include "../src/Circle.cpp"

#endif // CIRCLE_H_INCLUDED
