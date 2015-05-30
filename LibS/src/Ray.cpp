template <class T>
Ray<T>::Ray(const Vec2<T>& o, const Vec2<T>& d) : origin(o), direction(d.normalized())
{

}

template <class T>
template <class X>
Ray<T>::Ray(const Ray<X>& r) : origin(r.origin), direction(r.direction)
{

}
template <class T>
template <class X>
Ray<T>& Ray<T>::operator=(const Ray<X>& r)
{
    origin = r.origin;
    direction = r.direction;
}

template <class T>
Ray<T> Ray<T>::operator+(const Vec2<T>& v) const
{
    Ray<T> r(*this);
    r.origin += v;
    return r;
}
template <class T>
Ray<T>& Ray<T>::operator+=(const Vec2<T>& v)
{
    origin += v;
    return *this;
}
template <class T>
Ray<T> Ray<T>::operator-(const Vec2<T>& v) const
{
    Ray<T> r(*this);
    r.origin -= v;
    return r;
}
template <class T>
Ray<T>& Ray<T>::operator-=(const Vec2<T>& v)
{
    origin -= v;
    return *this;
}

template <class T>
void Ray<T>::setDirection(const Vec2<T>& d)
{
    direction = d.normalized();
}

template <class T>
void Ray<T>::translate(const Vec2<T>& v)
{
    origin.translate(v);
}
template <class T>
void Ray<T>::scale(const Vec2<T>& s)
{
    origin.scale(s);
    direction.scale(s);
    direction.normalize();
}
template <class T>
void Ray<T>::transform(const std::function<void(Vec2<T>&)>& transformationFunction)
{
    Vec2<T> somePointOnRay = origin + direction;
    transformationFunction(somePointOnRay);

    transformationFunction(origin);
    direction = somePointOnRay - origin;
}
template <class T>
void Ray<T>::transform(const Transformation2<T>& transformation)
{
    Vec2<T> somePointOnRay = origin + direction;
    transformation.transform(somePointOnRay);

    transformation.transform(origin);
    direction = somePointOnRay - origin;
}
template <class T>
Ray<T> Ray<T>::transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const
{
    Ray<T> copy(*this);
    copy.transform(transformationFunction);
    return copy;
}
template <class T>
Ray<T> Ray<T>::transformed(const Transformation2<T>& transformation) const
{
    Ray<T> copy(*this);
    copy.transform(transformation);
    return copy;
}

template <class T>
T Ray<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Ray<T>::nearestPointTo(const Vec2<T>& point) const
{
    Vec2<T> AO = point - origin;
    T t = AO.dot(direction) / direction.quadrance();
    t = std::max(t, T(0.0));
    return origin + direction * t;
}

template <class T>
Polyline<T> Ray<T>::asPolyline() const
{
    //dont know
    return Polyline<T>({origin});
}

template <class T>
std::unique_ptr<Shape2<T>> Ray<T>::clone() const
{
    return std::make_unique<Ray<T>>(*this);
}
