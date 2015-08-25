#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

template <class T>
class Circle : public Shape2<T>
{
public:
    Vec2<T> origin;
    T radius;

    Circle(){};
    Circle(const Vec2<T>& p1, T r);

    Circle(const Circle<T>& other){origin = other.origin; radius = other.radius;}
    Circle(Circle<T>&& other){origin = std::move(other.origin); radius = std::move(other.radius);}

    virtual ~Circle(){}

    Circle<T>& operator =(Circle<T> && other){origin = std::move(other.origin); radius = std::move(other.radius); return *this;}
    Circle<T>& operator =(const Circle<T>& other){origin = other.origin; radius = other.radius; return *this;}

    Circle<T> operator +(const Vec2<T>& v) const;
    Circle<T> operator -(const Vec2<T>& v) const;

    Circle<T>& operator +=(const Vec2<T>& v);
    Circle<T>& operator -=(const Vec2<T>& v);

    template <class T2>
    explicit operator Circle<T2>() const;

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Vec2<T> centerOfMass() const;
    T area() const;
};

typedef Circle<double> CircleD;
typedef Circle<float> CircleF;
typedef Circle<int> CircleI;

extern template class Circle<double>;
extern template class Circle<float>;
extern template class Circle<int>;

#include "../src/Circle.cpp"

#endif // CIRCLE_H_INCLUDED
