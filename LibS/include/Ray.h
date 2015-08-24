#ifndef RAY_H
#define RAY_H

template <class T>
class Ray : public Shape2<T>
{
public:
    Vec2<T> origin;
    Vec2<T> direction; //is expected to be normalized

    Ray() = default;
    Ray(const Vec2<T>& o, const Vec2<T>& d);

    Ray(const Ray<T>&) = default;
    template <class X>
    Ray(const Ray<X>& r);
    Ray(Ray<T>&&) = default;

    virtual ~Ray(){}

    Ray<T>& operator=(const Ray<T>&) = default;
    template <class X>
    Ray<T>& operator=(const Ray<X>& r);
    Ray<T>& operator=(Ray<T> &&) = default;

    Ray<T> operator+(const Vec2<T>& v) const;
    Ray<T> operator-(const Vec2<T>& v) const;

    Ray<T>& operator+=(const Vec2<T>& v);
    Ray<T>& operator-=(const Vec2<T>& v);

    void setDirection(const Vec2<T>& d);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;
};

typedef Ray<double> RayD;
typedef Ray<float> RayF;
typedef Ray<int> RayI;

extern template class Ray<double>;
extern template class Ray<float>;
extern template class Ray<int>;

#include "../src/Ray.cpp"

#endif // RAY_H
