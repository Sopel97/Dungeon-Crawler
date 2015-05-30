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

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Ray<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Ray<T> transformed(const Transformation2<T>& transformation) const;

    virtual T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

#ifndef GEOMETRY_LIGHT
    virtual bool intersects(const Shape2<T>* other) const {return other->intersects(*this);}
    virtual bool contains(const Shape2<T>* other) const {return other->isContained(*this);}
    virtual bool isContained(const Shape2<T>* other) const {return other->contains(*this);}
#endif // GEOMETRY_LIGHT
    SHAPE2_DOUBLE_DISPATCHING_METHODS

    virtual std::unique_ptr<Shape2<T>> clone() const;
};

typedef Ray<double> RayD;
typedef Ray<float> RayF;
typedef Ray<int> RayI;

#include "../src/Ray.cpp"

#endif // RAY_H
